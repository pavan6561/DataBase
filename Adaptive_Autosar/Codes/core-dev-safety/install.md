## Adaptive-Core (Ara-Core)

<h3> Yocto Build </h3>

```shell
1. Set up build environment
$sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib build-essential chrpath socat libsdl1.2-dev xterm repo  curl tree iptables tree

2. Create a directory, eg: mkdir core-build
*Inside core-build clone the core, log, platform,util yocto-layers repositories from dev-sqm-r20.11 branch into core-build : 
* core-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00056/core.git
* core-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00056/log.git
* core-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00056/platform.git
* core-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00056/yocto-layers.git
* core-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00056/util.git

3. Checkout from dev-sqm-r20.11 branch.
* core-build$ cd core
* core-build/core$  git checkout -b dev-sqm-r20.11
* core-build/log$  git checkout -b dev-alpha-r20.11
* core-build/platform$  git checkout -b dev
* core-build/yocto-layers$  git checkout -b dev
* core-build/util$  git checkout -b dev-alpha-r20.11

4. Clone the other supporting repositories from AUTOSAR using by running following script
* core-build$ git clone https://github.com/oci-labs/meta-opendds meta-opendds
git -c advice.detachedHead=false -C meta-opendds checkout f82677dd242fd4f7c453ffa64ddeb7dbbce28dbe

* core-build$ git clone https://github.com/openembedded/meta-openembedded meta-openembedded
git -c advice.detachedHead=false -C meta-openembedded checkout 2b5dd1eb81cd08bc065bc76125f2856e9383e98b

* core-build$ git clone https://git.yoctoproject.org/git/poky poky
git -c advice.detachedHead=false -C poky checkout d88d62c20d7d8da85f02edb170dae0280624ad7e
    It will clone poky, meta-openembedded, meta-opendds

5. First create the initial default build directory.
*core-build$ source poky/oe-init-build-env

6. From there, add the layers meta-ara , meta-oe & meta-python.
*core-build/build$ bitbake-layers add-layer ../meta-openembedded/meta-oe
*core-build/build$ bitbake-layers add-layer ../meta-openembedded/meta-python
*core-build/build$ bitbake-layers add-layer ../meta-opendds
*core-build/build$ bitbake-layers add-layer ../yocto-layers/meta-ara

7. Finally, add a suitable location for Yocto <i>downloads</i> and <i>sstate-cache</i> folders to conf/local.conf
*core-build/build$ echo "MACHINE ??= \"qemux86-64\"" >> conf/local.conf
*core-build/build$ echo "DL_DIR = \"$HOME/yocto/downloads\"" >> conf/local.conf
*core-build/build$ echo "SSTATE_DIR = \"$HOME/yocto/sstate-cache\"" >> conf/local.conf
*core-build/build$ echo 'PREFERRED_VERSION_opendds="1.0+git%"' >> conf/local.conf
*core-build/build$ echo 'PREFERRED_VERSION_opendds-native="1.0+git%"' >> conf/local.conf
*core-build/build$ echo 'PREFERRED_VERSION_nativesdk-opendds="1.0+git%"' >> conf/local.conf

8. Command for building the SM image
*core-build/build$ bitbake ara-core
*core-build/build$ bitbake core-types
*core-build/build$ bitbake apd-timer
*core-build/build$ bitbake config-reader

9. Command for launching image
* Not applicable for library.

10. Run Execution Manager on qemu instance.
* Not applicable for library.
```

## FAQ
NA