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

#include "routerequest.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <cstring>
#include <string>

#define BUFFERSIZE 512

RouteRequest::RouteRequest(const std::string& address, const std::string& interface) :
    adr(address), iface(interface)
{
}

RouteRequest::~RouteRequest()
{
  free(buffer);
}

int RouteRequest::addattr_l(struct nlmsghdr* n, unsigned int maxlen, int type, void* data, int datalen)
{
  unsigned int len = RTA_LENGTH(datalen);
  struct rtattr* rta;
  if ((NLMSG_ALIGN(n->nlmsg_len) + len) > maxlen)
  {
    std::cout << "NULL arg detected!" << std::endl;
    return -1;
  }
  rta = (struct rtattr*)(((unsigned char*)n) + NLMSG_ALIGN(n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  if (data != NULL)
  {
    memcpy(RTA_DATA(rta), data, datalen);
    n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + len;
  }
  else
  {
    rta->rta_len = 4;
  }
  return 0;
}

int RouteRequest::addattr32(struct nlmsghdr* n, unsigned int maxlen, int type, uint32_t data)
{
  unsigned int len = RTA_LENGTH(4);
  struct rtattr* rta;

  if ((NLMSG_ALIGN(n->nlmsg_len) + len) > maxlen)
  {
    return -1;
  }
  rta = (struct rtattr*)(((unsigned char*)n) + NLMSG_ALIGN(n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  *(reinterpret_cast<uint32_t*>(RTA_DATA(rta))) = data;
  n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + len;
  return 0;
}

int RouteRequest::initRouteRequest()
{
  buffer = (unsigned char*)malloc(BUFFERSIZE); /* allocate memory for buffer and set everything to 0*/
  if (NULL == buffer)
  {
    return -1;
  }
  memset(buffer, 0, BUFFERSIZE);
  nl_msg_hdr = (struct nlmsghdr*)(buffer); /* handle previously allocated memory as netlink message header */
  rt_msg
      = (struct rtmsg*)NLMSG_DATA(nl_msg_hdr); /* get pointer to payload associated with the passed header */
  memset(&(addr), 0, sizeof(struct sockaddr_nl)); /* set address memory to all zeros */
  memset(&(msg_hdr), 0, sizeof(struct msghdr)); /* set message header memory to all zeros */
  memset(&(iov), 0, sizeof(struct iovec)); /* set iovec memory to all zeros */

  return 0;
}

int RouteRequest::fillNetlinkMessageHeader(unsigned flags)
{
  nl_msg_hdr->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); /* calculate length of the message */
  nl_msg_hdr->nlmsg_type = RTM_NEWROUTE; /* message to create a new route */
  nl_msg_hdr->nlmsg_flags = (NLM_F_REQUEST | flags); /* message is a request */
  nl_msg_hdr->nlmsg_pid = 0; /* send request to the kernel */
  nl_msg_hdr->nlmsg_seq = 0; /* handle the message sequence */

  return 0;
}

int RouteRequest::fillMessage()
{
  rt_msg->rtm_family = AF_INET; /* use IPv4 addresses */
  rt_msg->rtm_dst_len = 4; /* 4 bytes are needed for their representation*/
  rt_msg->rtm_src_len = 0; /* currently we don't use the source address */
  rt_msg->rtm_tos = 0; /* no Type-Of-Service filter*/
  rt_msg->rtm_table = RT_TABLE_MAIN; /* FIXME table number unspecified */
  rt_msg->rtm_protocol = RTPROT_BOOT; /* FIXME route installed by administrator */
  rt_msg->rtm_scope = RT_SCOPE_LINK; /* FIXME global route */
  rt_msg->rtm_type = RTN_UNICAST; /* FIXME a multicast route */
  rt_msg->rtm_flags = 0; /* no flags */
  return 0;
}

int RouteRequest::fillMessageHeader()
{
  msg_hdr.msg_name = reinterpret_cast<void*>(&(addr)); /* FIXME */
  msg_hdr.msg_namelen = sizeof(struct sockaddr_nl); /* FIXME */
  msg_hdr.msg_iov = &(iov); /* FIXME */
  msg_hdr.msg_iovlen = 1; /* FIXME */
  msg_hdr.msg_control = NULL; /* FIXME */
  msg_hdr.msg_controllen = 0; /* FIXME */
  msg_hdr.msg_flags = 0; /* FIXME */
  addr.nl_family = AF_NETLINK; /* FIXME */
  return 0;
}

int RouteRequest::fillPayload(const char* destination_address_string, const char* interface_name)
{
  int result = 0;
  int interface_index = -1;
  unsigned char destination_address[4] = {0};

  /* convert IP to binary format */
  if (0 >= (result = inet_pton(AF_INET, destination_address_string, &destination_address)))
  {
    std::cout << "conversion of destination_address failed : " << result << std::endl;
    return -1;
  }

  /* add address to payload */
  if (0 != (result = addattr_l(nl_msg_hdr, BUFFERSIZE, RTA_DST, &destination_address, 4)))
  {
    std::cout << "adding destination_address failed : " << result << std::endl;
    return -2;
  }

  /* get interface index by name */
  (NULL != interface_name) ? result = interface_index = if_nametoindex(interface_name) : result = -1;
  if (0 == result)
  {
    std::cout << "getting interface index by name failed : " << result << std::endl;
    return -3;
  }

  /* add interface index to payload */
  if (0 != (result = addattr32(nl_msg_hdr, BUFFERSIZE, RTA_OIF, interface_index)))
  {
    std::cout << "adding output interface failed : " << result << std::endl;
    return -4;
  }

  // FIXME document what happens here
  iov.iov_base = reinterpret_cast<void*>(nl_msg_hdr);
  iov.iov_len = nl_msg_hdr->nlmsg_len;
  nl_msg_hdr->nlmsg_flags |= NLM_F_ACK; /* request acknowledgement from receiver */
  unsigned int* sequence_number = &(nl_msg_hdr->nlmsg_seq);
  (*sequence_number)++; /* manually increase the sequence number */

  return 0;
}

int RouteRequest::construct()
{
  //  FIXME check if the given string has the right format
  int res = 0;

  /* PERFORM INIT ON REQUEST STRUCT */
  if (0 != (res = initRouteRequest()))
  {
    return res;
  }

  /* SPECIFY NETLINK MESSAGE HEADER*/
  if (0 != (res = fillNetlinkMessageHeader(NLM_F_EXCL | NLM_F_CREATE)))
  {
    return res;
  }

  /* SPECIFY MESSAGE HEADER */
  if (0 != (res = fillMessageHeader()))
  {
    return res;
  }

  /* SPECIFY MESSAGE */
  if (0 != (res = fillMessage()))
  {
    return res;
  }

  /* CONSTRUCT PAYLOAD */
  if (0 != (res = fillPayload(adr.c_str(), iface.c_str())))
  {
    return res;
  }
  return res;
}

int RouteRequest::send()
{
  /* CREATE SOCKET AND SEND MESSAGE */
  if (0 > (rtnetlink_socket = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)))
  { /* try to create socket */
    std::cout << "socket creation failed : " << strerror(errno) << std::endl;
    return -1;
  }

  /* SEND THE MESSAGE */
  if (0 > sendmsg(rtnetlink_socket, &(msg_hdr), 0))
  {
    std::cout << "message could not be sent : " << strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

/*  FIXME this function gets the result(s) back from the kernel and checks if everything went
 *  right, if not errors will be printed in human readable format
 */
int RouteRequest::receiveResult()
{
  /* WAIT FOR RESPONSE */
  unsigned int received_bytes = 0;
  struct nlmsghdr* nl_msg_hdr;

  if (static_cast<int>(sizeof(*nl_msg_hdr)) > (received_bytes = recvmsg(rtnetlink_socket, &(msg_hdr), 0)))
  {
    std::cout << "reading from socket failed : " << strerror(errno) << std::endl;
    return -1;
  }

  for (nl_msg_hdr = (struct nlmsghdr*)buffer; received_bytes >= static_cast<int>(sizeof(*nl_msg_hdr));
       nl_msg_hdr
       = (struct nlmsghdr*)(reinterpret_cast<char*>(nl_msg_hdr) + NLMSG_ALIGN(nl_msg_hdr->nlmsg_len)))
  {
    if (nl_msg_hdr->nlmsg_type == NLMSG_ERROR)
    {
      // still ok
      struct nlmsgerr* err = (struct nlmsgerr*)NLMSG_DATA(nl_msg_hdr);
      if (err->error != 0)
      {
        std::cout << "network_init: could not complete command : " << strerror(-err->error) << std::endl;
        return -1;
      }
    }
    received_bytes -= NLMSG_ALIGN(nl_msg_hdr->nlmsg_len);
  }
  return 0;
}
