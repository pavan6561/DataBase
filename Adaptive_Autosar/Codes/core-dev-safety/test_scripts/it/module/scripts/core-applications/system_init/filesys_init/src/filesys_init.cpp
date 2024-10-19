// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

#include "filesys_init.h"

#include <string.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <iostream>

namespace filesysinit
{
/**
 * Error handler for the libmount fstab parser.
 */
int libmountErrCb(struct libmnt_table* tb __attribute__((__unused__)), const char* filename, int line)
{
  if (filename)
  {
    std::cout << "filesys_init: " << filename << ": parse error: ignore entry at line " << line << std::endl;
  }
  return 0;
}

FilesysInit::FilesysInit() noexcept : mount_ctx_(mnt_new_context(), mnt_free_context)
{
}

FilesysInit::~FilesysInit() noexcept
{
}

void FilesysInit::Initialize()
{
  mnt_init_debug(0);

  if (!mount_ctx_)
  {
    throw std::runtime_error("filesys_init: failed to initilize libmount.");
  }

  // Register C callback errorhandler for fstab parser
  (void)mnt_context_set_tables_errcb(mount_ctx_.get(), &libmountErrCb);
}

void FilesysInit::Run()
{
  LibmountIterType itr(mnt_new_iter(MNT_ITER_FORWARD), mnt_free_iter);
  if (!itr)
  {
    throw std::runtime_error("filesys_init: failed to initialize libmount iterator.");
  }

  int mntrc, ignored;
  struct libmnt_fs* fs;

  while (LibmountNextMount(itr, fs, mntrc, ignored) == 0)
  {
    const char* tgt = mnt_fs_get_target(fs);

    if (ignored)
    {
      std::cout << "filesys_init: " << tgt << ((ignored == 1) ? ": ignored" : ": already mounted")
                << std::endl;
    }
    else if (mnt_context_get_status(mount_ctx_.get()) && IsMountExitSuccess(mntrc))
    {
      std::cout << "filesys_init: " << tgt << ": successfully mounted" << std::endl;
    }
  }
}

void FilesysInit::Shutdown()
{
}

/**
 * Performs the mounting (by utilizing libmount) of the fstab entry referenced by the iterator.
 */
int FilesysInit::LibmountNextMount(
    LibmountIterType const& libmntItr, struct libmnt_fs*& fs, int& mntrc, int& ignored)
{
  ignored = 0;
  mntrc = 0;

  struct libmnt_context* ctx = mount_ctx_.get();

  (void)mnt_reset_context(ctx);

  struct libmnt_table* fstab;
  int rc = mnt_context_get_fstab(ctx, &fstab);
  if (rc)
  {
    return rc;
  }

  rc = mnt_table_next_fs(fstab, libmntItr.get(), &fs);
  if (rc != 0)
  {
    return rc; /* no more entries (or error) */
  }

  const char* opt = mnt_fs_get_user_options(fs);
  const char* tgt = mnt_fs_get_target(fs);

  /*  ignore swap */
  if (mnt_fs_is_swaparea(fs) ||
      /* ignore root filesystem */
      (tgt && (strcmp(tgt, "/") == 0 || strcmp(tgt, "root") == 0)) ||
      /* ignore noauto filesystems */
      (opt && mnt_optstr_get_option(opt, "noauto", NULL, NULL) == 0))
  {
    ignored = 1;
    return 0;
  }

  /*
     * Ignore already mounted filesystems.
     * Except mount information cannot be fetched because /proc is not mounted
     */
  int mounted = 0;
  rc = mnt_context_is_fs_mounted(ctx, fs, &mounted);
  if (rc && (rc != -ENOENT))
  {
    return rc;
  }

  if (mounted)
  {
    ignored = 2;
    return 0;
  }

  /*
     * Try to create mount point directory.
     * Instead of checking for existence first, just call mkdir and ignore
     * any error. Only log the (more uncommon) case of succcess, i.e. when
     * the mount point still needs creation.
     */
  rc = mkdir(tgt, S_IRWXU | S_IRWXG | S_IRWXO);
  if (rc == 0)
  {
    std::cout << "filesys_init: " << tgt << ": mount point did not exist, created it" << std::endl;
  }

  /* do the mount */
  rc = mnt_context_set_fs(ctx, fs);
  if (!rc)
  {
    rc = mnt_context_mount(ctx);
    mntrc = rc;
  }

  return 0;
}

/**
 * Evaluates if the mounting routine was successful or an error raised by libmount or underlying
 * mount(2) syscalls.
 */
bool FilesysInit::IsMountExitSuccess(const int& rc)
{
  struct libmnt_context* ctx = mount_ctx_.get();
  if (rc == 0 && mnt_context_get_status(ctx) == 1)
  {
    /* libmount success && syscall success */
    return true;
  }

  const char* tgt = mnt_context_get_target(ctx);
  const char* src = mnt_context_get_source(ctx);

  unsigned long uflags = 0;
  (void)mnt_context_get_user_mflags(ctx, &uflags); /* userspace flags */
  int syserr = mnt_context_get_syscall_errno(ctx);

  /*
     * libmount errors (extra library checks)
     */
  if (!syserr)
  {
    switch (rc)
    {
    case -EPERM:
      std::cout << "libmount error: only root can mount " << src << " on " << tgt << std::endl;
      return false;
    case -EBUSY:
      std::cout << "libmount error: " << src << " is already mounted" << std::endl;
      return false;
    case -MNT_ERR_NOFSTAB:
      if (mnt_context_is_swapmatch(ctx))
      {
        std::cout << "libmount error: can't find " << (src ? src : tgt) << " in " << mnt_get_fstab_path()
                  << std::endl;
        return false;
      }
      /* source/target explicitly defined */
      if (tgt)
      {
        std::cout << "libmount error: can't find mountpoint " << tgt << " in " << mnt_get_fstab_path()
                  << std::endl;
      }
      else
      {
        std::cout << "libmount error: can't find mount source " << src << " in " << mnt_get_fstab_path()
                  << std::endl;
      }
      return false;
    case -MNT_ERR_NOSOURCE:
      if (uflags & MNT_MS_NOFAIL)
      {
        return false;
      }
      if (src)
      {
        std::cout << "libmount error: can't find " << src << std::endl;
      }
      else
      {
        std::cout << "libmount error: mount source not defined" << std::endl;
      }
      return false;
    case -MNT_ERR_MOUNTOPT:
      std::cout << "libmount error: mount source not defined" << std::endl;
      return false;
    case -MNT_ERR_LOOPDEV:
      std::cout << "libmount error: failed to setup loop device " << src << std::endl;
      return false;
    default:
      std::cout << "libmount error: " << tgt << " mount failed, unknown reason" << std::endl;
      return false;
    }
  }
  else if (syserr == 0)
  {
    /* mount(2) syscall success, but something else failed (probably error in mtab processing)
         */
    if (rc < 0)
    {
      std::cout << "mount(2) syscall success, but something else failed" << src << std::endl;
    }
    return false;
  }

  struct stat st;
  unsigned long mflags = 0;
  (void)mnt_context_get_mflags(ctx, &mflags); /* mount(2) flags */

  /*
     * mount(2) errors (extra syscall checks)
     */
  switch (syserr)
  {
  case EPERM:
    if (geteuid() == 0)
    {
      std::cout << "mount(2) error: permission denied" << std::endl;
    }
    else
    {
      std::cout << "mount(2) error: must be superuser to use mount" << std::endl;
    }
    break;
  case EBUSY:
  {
    struct libmnt_table* tb;

    if (mflags & MS_REMOUNT)
    {
      std::cout << "mount(2) error: " << tgt << " is busy" << std::endl;
      break;
    }

    std::cout << "mount(2) error: " << src << " is already mounted or " << tgt << " is busy" << std::endl;

    if (src && mnt_context_get_mtab(ctx, &tb) == 0)
    {
      LibmountIterType itr(mnt_new_iter(MNT_ITER_FORWARD), mnt_free_iter);
      if (!itr)
      {
        throw std::runtime_error("filesys_init: failed to initialize libmount iterator.");
      }

      struct libmnt_fs* fs;
      while (mnt_table_next_fs(tb, itr.get(), &fs) == 0)
      {
        const char *s = mnt_fs_get_srcpath(fs), *t = mnt_fs_get_target(fs);

        if (t && s && mnt_fs_streq_srcpath(fs, src))
        {
          std::cout << "mount(2) error: " << s << " is already mounted on " << t << std::endl;
        }
      }
    }
  }
  break;
  case ENOENT:
    if (tgt && lstat(tgt, &st))
    {
      std::cout << "mount(2) error: mount point " << tgt << " does not exist" << std::endl;
    }
    else if (tgt && stat(tgt, &st))
    {
      std::cout << "mount(2) error: mount point " << tgt << " is a symbolic link to nowhere" << std::endl;
    }
    else if (src && stat(src, &st))
    {
      if (uflags & MNT_MS_NOFAIL)
      {
        return true;
      }
      std::cout << "mount(2) error: special device " << src << " does not exist" << std::endl;
    }
    else
    {
      std::cout << "mount(2) error: code " << syserr << std::endl;
    }
    break;
  case ENOTDIR:
    if (stat(tgt, &st) || !S_ISDIR(st.st_mode))
    {
      std::cout << "mount(2) error: mount point " << tgt << " is not a directory" << std::endl;
    }
    else if (src && stat(src, &st) && errno == ENOTDIR)
    {
      if (uflags & MNT_MS_NOFAIL)
      {
        return true;
      }
      std::cout << "mount(2) error: special device " << src
                << " does not exist (a path prefix is not a directory)" << std::endl;
    }
    else
    {
      std::cout << "mount(2) error: code " << syserr << std::endl;
    }
    break;
  case EINVAL:
    if (mflags & MS_REMOUNT)
    {
      std::cout << "mount(2) error: " << tgt << " not mounted or bad option" << std::endl;
    }
    else if (rc == -MNT_ERR_APPLYFLAGS)
    {
      std::cout << "mount(2) error: " << tgt << " is not mountpoint or bad option" << std::endl;
    }
    else
    {
      std::cout << "mount(2) error: wrong fs type, bad option, bad superblock on " << src
                << "\nmissing codepage or helper program, or other error" << std::endl;
    }
    break;
  case EMFILE:
    std::cout << "mount(2) error: mount table full" << std::endl;
    break;
  case EIO:
    std::cout << "mount(2) error: " << src << " can't read superblock" << std::endl;
    break;
  case ENODEV:
    if (mnt_context_get_fstype(ctx))
    {
      std::cout << "mount(2) error: unknown filesystem type " << mnt_context_get_fstype(ctx) << std::endl;
    }
    else
    {
      std::cout << "mount(2) error: unknown filesystem type" << std::endl;
    }
    break;
  case ENOTBLK:
    if (uflags & MNT_MS_NOFAIL)
    {
      return true;
    }
    if (stat(src, &st))
    {
      std::cout << "mount(2) error: " << src << " is not a block device, and stat(2) fails?" << std::endl;
    }
    else if (S_ISBLK(st.st_mode))
    {
      std::cout << "mount(2) error: the kernel does not recognize " << src
                << "as a block device\n(maybe `modprobe driver'?)" << std::endl;
    }
    else if (S_ISREG(st.st_mode))
    {
      std::cout << "mount(2) error: " << src << " is not a block device (maybe try `-o loop'?)" << std::endl;
    }
    else
    {
      std::cout << "mount(2) error: " << src << " is not a block device" << std::endl;
    }
    break;
  case ENXIO:
    if (uflags & MNT_MS_NOFAIL)
    {
      return true;
    }
    std::cout << "mount(2) error: " << src << " is not a valid block device" << std::endl;
    break;
  case EACCES:
  case EROFS:
    if (mflags & MS_RDONLY)
    {
      std::cout << "mount(2) error: cannot mount " << src << " read-only" << std::endl;
    }
    else if (mflags & MS_REMOUNT)
    {
      std::cout << "mount(2) error: cannot remount " << src << " read-write, is write-protected" << std::endl;
    }
    else if (mflags & MS_BIND)
    {
      std::cout << "mount(2) error: mount " << src << " on " << tgt << " failed" << std::endl;
    }
    break;
  case ENOMEDIUM:
    std::cout << "mount(2) error: no medium found on " << src << std::endl;
    break;

  default:
    std::cout << "mount(2) error: mount " << src << " on " << tgt << " failed, unknown reason" << std::endl;
    break;
  }

  return false;
}

} /* namespace filesysinit */
