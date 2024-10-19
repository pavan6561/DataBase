// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : uuid_impl.h
//
// Purpose     : This file contains definitions related to uuidImpl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_UTILS_INC_PUBLIC_UUID_IMPL_H_
#define CRYPTO_SOURCE_UTILS_INC_PUBLIC_UUID_IMPL_H_

#include <chrono>
#include <cstdint>
#include <cstring>

#include <ara/core/string.h>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iomanip>
#include <iostream>
#include <net/if.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ara/crypto/common/uuid.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief uuidImpl class handles the generation of uuid.
class uuidImpl
{
 public:
  /// @brief method to get the MAC address of the system.
  static void GetMacAddress(uint8_t* macbuff)
  {
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1)
    { /* handle error*/
    };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1)
    { /* handle error */
    }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it)
    {
      strcpy(ifr.ifr_name, it->ifr_name);
      if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
      {
        if (!(ifr.ifr_flags & IFF_LOOPBACK))
        { // don't count loopback
          if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0)
          {
            success = 1;
            break;
          }
        }
      }
      else
      { /* handle error */
      }
    }

    //unsigned char mac_address[6];

    if (success)
      memcpy(macbuff, ifr.ifr_hwaddr.sa_data, 6);
  }

  /// @brief method to convert uuid to string.
  static ara::core::String ConvertaraUuidtoStringUuid(Uuid objectuuId)
  {
    uint8_t uuidValue[16];
    char strValueForUuid[37];

    for (size_t index = 0; index < 16; index++)
    {
      if (index < 8)
      {
        uuidValue[index] = (objectuuId.mQwordMs >> (index * 8)) & 0xFF;
      }
      else
      {
        uuidValue[index] = (objectuuId.mQwordLs >> ((index - 8) * 8)) & 0xFF;
      }
    }
    sprintf(strValueForUuid, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
      uuidValue[0], uuidValue[1], uuidValue[2], uuidValue[3], uuidValue[4], uuidValue[5], uuidValue[6],
      uuidValue[7], uuidValue[8], uuidValue[9], uuidValue[10], uuidValue[11], uuidValue[12], uuidValue[13],
      uuidValue[14], uuidValue[15]);

    ara::core::String stringUuid{ strValueForUuid };

    return stringUuid;
  }

  /// @brief method to generate new uuid.
  static Uuid GeneratearaUuid()
  {
    uint64_t timeinNanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch())
                                   .count();
    uint8_t uuidValue[16];
    Uuid generatedUuid;

    for (size_t index = 0; index < 10; index++)
    {
      if (index < 6)
      {
        uuidValue[index] = (timeinNanoSeconds >> (index * 8)) & 0xFF;
      }
      else if (index == 6)
      {
        uuidValue[index] = ((timeinNanoSeconds >> (index * 8)) & 0xF0) >> 4;
        uuidValue[index] |= 0x10;
        uuidValue[index + 1] = (((timeinNanoSeconds >> (index * 8)) & 0x0F)) << 4;
      }
      else if (index == 7)
      {
        uuidValue[index] |= ((timeinNanoSeconds >> (index * 8)) & 0xF0) >> 4;
        uuidValue[index + 1] = 0xb0;
        uuidValue[index + 1] |= (((timeinNanoSeconds >> (index * 8)) & 0x0F));
      }
      else if (index != 8)
      {
        uuidValue[index] = 0xbc;
      }
    }
    GetMacAddress(&uuidValue[10]);

    for (size_t index = 16; index > 0; index--)
    {
      if (index < 9)
      {
        generatedUuid.mQwordMs <<= 8;
        generatedUuid.mQwordMs |= uuidValue[index];
      }
      else
      {
        generatedUuid.mQwordLs <<= 8;
        generatedUuid.mQwordLs |= uuidValue[index];
      }
    }
    return generatedUuid;
  }

  /// @brief method to convert uuid from string to ara::uuid.
  static Uuid UuidStrToAraUuid(const ara::core::String& uuid)
  {
    boost::uuids::uuid convertedUuid;
    ara::crypto::Uuid targetUid;
    try
    {
      convertedUuid = boost::lexical_cast<boost::uuids::uuid>(uuid);
    }
    catch (boost::bad_lexical_cast& e)
    {
      return targetUid;
    }
    static_cast<void>(memcpy(&targetUid.mQwordMs, convertedUuid.data, sizeof(std::uint64_t)));
    static_cast<void>(
      memcpy(&targetUid.mQwordLs, (convertedUuid.data + sizeof(std::uint64_t)), sizeof(std::uint64_t)));
    return targetUid;
  }
};

} // namespace crypto
} // namespace ara
#endif