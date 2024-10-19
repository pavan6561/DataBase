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

#ifndef NETWORK_INIT_INCLUDE_NETWORKINIT_ROUTEREQUEST_H_
#define NETWORK_INIT_INCLUDE_NETWORKINIT_ROUTEREQUEST_H_

#include <asm/types.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <iostream>
#include <string>

class RouteRequest
{
 private:
  struct nlmsghdr* nl_msg_hdr;
  struct msghdr msg_hdr;
  struct rtmsg* rt_msg;
  struct sockaddr_nl addr;
  struct iovec iov;
  int rtnetlink_socket = -1;
  std::string adr;
  std::string iface;
  unsigned char* buffer;

  int addattr_l(struct nlmsghdr* n, unsigned int maxlen, int type, void* data, int datalen);
  int addattr32(struct nlmsghdr* n, unsigned int maxlen, int type, uint32_t data);
  int initRouteRequest();
  int fillNetlinkMessageHeader(unsigned flags);
  int fillMessage();
  int fillMessageHeader();
  int fillPayload(const char* destination_address_string, const char* interface_name);

 public:
  RouteRequest(const std::string& address, const std::string& interface);
  ~RouteRequest();
  int construct();
  int send();
  int receiveResult();
};
#endif // NETWORK_INIT_INCLUDE_NETWORKINIT_ROUTEREQUEST_H_
