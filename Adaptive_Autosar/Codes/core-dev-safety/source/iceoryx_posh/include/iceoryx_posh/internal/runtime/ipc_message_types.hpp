// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef IOX_POSH_RUNTIME_IPC_MESSAGE_TYPES_HPP
#define IOX_POSH_RUNTIME_IPC_MESSAGE_TYPES_HPP


#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "iceoryx_hoofs/cxx/convert.hpp"

namespace iox
{
namespace runtime
{

enum class IpcMessageType : int32_t
{
    BEGIN = -1,
    NOTYPE = 0,
    REG, // register app
    REG_ACK,
    CREATE_PUBLISHER,
    CREATE_PUBLISHER_ACK,
    CREATE_SUBSCRIBER,
    CREATE_SUBSCRIBER_ACK,
    CREATE_CLIENT,
    CREATE_CLIENT_ACK,
    CREATE_SERVER,
    CREATE_SERVER_ACK,
    CREATE_INTERFACE,
    CREATE_INTERFACE_ACK,
    CREATE_CONDITION_VARIABLE,
    CREATE_CONDITION_VARIABLE_ACK,
    CREATE_NODE,
    CREATE_NODE_ACK,
    KEEPALIVE,
    TERMINATION,
    TERMINATION_ACK,
    PREPARE_APP_TERMINATION,
    PREPARE_APP_TERMINATION_ACK,
    ERROR,
    APP_WAIT,
    WAKEUP_TRIGGER,
    REPLAY,
    MESSAGE_NOT_SUPPORTED,
    // etc..
    END,
    GETINITIALSTATE = 28,
    STATECHANGEACK = 29,
    SMHANDLERACK = 30,
    RECOVERYHANDLERSUCCESS,
    WDGALIVENOTIFICATION,
    FIREWDGREACTION,
    RECOVERYHANDLERFAILED
};

/// If IpcMessageType::ERROR, this is the sub type for details about the error
enum class IpcMessageErrorType : int32_t
{
    BEGIN,
    NOTYPE,
    /// A publisher could not be created unique
    NO_UNIQUE_CREATED,
    INTERNAL_SERVICE_DESCRIPTION_IS_FORBIDDEN,
    REQUEST_PUBLISHER_INVALID_RESPONSE,
    REQUEST_PUBLISHER_WRONG_IPC_MESSAGE_RESPONSE,
    REQUEST_PUBLISHER_NO_WRITABLE_SHM_SEGMENT,
    REQUEST_SUBSCRIBER_INVALID_RESPONSE,
    REQUEST_SUBSCRIBER_WRONG_IPC_MESSAGE_RESPONSE,
    REQUEST_CLIENT_INVALID_RESPONSE,
    REQUEST_CLIENT_WRONG_IPC_MESSAGE_RESPONSE,
    REQUEST_CLIENT_NO_WRITABLE_SHM_SEGMENT,
    REQUEST_SERVER_INVALID_RESPONSE,
    REQUEST_SERVER_WRONG_IPC_MESSAGE_RESPONSE,
    REQUEST_SERVER_NO_WRITABLE_SHM_SEGMENT,
    REQUEST_CONDITION_VARIABLE_INVALID_RESPONSE,
    REQUEST_CONDITION_VARIABLE_WRONG_IPC_MESSAGE_RESPONSE,
    PUBLISHER_LIST_FULL,
    SUBSCRIBER_LIST_FULL,
    CLIENT_LIST_FULL,
    SERVER_LIST_FULL,
    CONDITION_VARIABLE_LIST_FULL,
    EVENT_VARIABLE_LIST_FULL,
    NODE_DATA_LIST_FULL,
    END,
};

#if 0
/// @brief Converts a string to the message type enumeration
/// @param[in] str string to convert
template<typename T>
T stringToIpcMessageType(const char* str) noexcept;

/// @brief Converts a message type enumeration value into a string
/// @param[in] msg enum value to convert
template<typename T>
std::string IpcMessageTypeToString(const T msg) noexcept;

/// @brief Converts a string to the message error type enumeration
/// @param[in] str string to convert
template<typename T>
T stringToIpcMessageErrorType(const char* str) noexcept;

/// @brief Converts a message error type enumeration value into a string
/// @param[in] msg enum value to convert
template<typename T>
std::string IpcMessageErrorTypeToString(const T msg) noexcept;
#endif

template<typename T>
T stringToIpcMessageType(const char* str) noexcept
{
    typename std::underlying_type<T>::type msg;
    bool noError = iox::cxx::convert::stringIsNumber(str, iox::cxx::convert::NumberType::INTEGER);
    noError &= noError ? (cxx::convert::fromString(str, msg)) : false;
    noError &= noError ? !(static_cast< typename std::underlying_type<T>::type>(T::BEGIN) >= msg
                           || static_cast<typename std::underlying_type<T>::type>(T::END) <= msg)
                       : false;
    return noError ? (static_cast<T>(msg)) : T::NOTYPE;
}

template<typename T>
std::string IpcMessageTypeToString(const T msg) noexcept
{
    return cxx::convert::toString(static_cast<typename std::underlying_type<T>::type>(msg));
}

template<typename T>
T stringToIpcMessageErrorType(const char* str) noexcept
{
    typename std::underlying_type<T>::type msg;
    bool noError = cxx::convert::stringIsNumber(str, cxx::convert::NumberType::INTEGER);
    noError &= noError ? (cxx::convert::fromString(str, msg)) : false;
    noError &= noError
                   ? !(static_cast<typename std::underlying_type<T>::type>(T::BEGIN) >= msg
                       || static_cast<typename std::underlying_type<T>::type>(T::END) <= msg)
                   : false;
    return noError ? (static_cast<T>(msg)) : T::NOTYPE;
}

template<typename T>
std::string IpcMessageErrorTypeToString(const T msg) noexcept
{
    return cxx::convert::toString(static_cast<typename std::underlying_type<T>::type>(msg));
}

} // namespace runtime
} // namespace iox

#endif // IOX_POSH_RUNTIME_IPC_MESSAGE_TYPES_HPP
