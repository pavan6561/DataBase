// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
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

#ifndef UTILS_IPC_INC_IPC_MESSAGE_TYPES_HPP
#define UTILS_IPC_INC_IPC_MESSAGE_TYPES_HPP

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "iceoryx_hoofs/cxx/convert.hpp"
namespace ara
{
namespace core
{
namespace ipc
{
/// @brief IpcMessageType class of enum type.
/// @design 613183 614029
enum class IpcMessageType : int32_t
{
  kBegin = -1,
  kNoType = 0,
  kReg = 2, // register app
  kRegAck = 3,
  kCreatePublisher = 4,
  kCreatePublisherAck = 5,
  kCreateSubscriber = 6,
  kCreateSubscriberAck = 7,
  kCreateClient = 8,
  kCreateClientAck = 9,
  kCreateServer = 10,
  kCreateServerAck = 11,
  kCreateInterface = 12,
  kCreateInterfaceAck = 13,
  kCreateConditionVariable = 14,
  kCreateConditionVariableAck = 15,
  kCreateNode = 16,
  kCreateNodeAck = 17,
  kKeepAlive = 18,
  kTermination = 19,
  kTerminationAck = 20,
  kPrepareAppTermination = 21,
  kPrepareAppTerminationAck = 22,
  kError = 23,
  kAppWait = 24,
  kWakeupTrigger = 25,
  kReplay = 26,
  kMessageNotSupported = 27,
  // etc..
  kEnd = 35,
  kGetInitialState = 28,
  kStateChangeAck = 29,
  kSmHandkerAck = 30,
  kRecoveryHandlerSuccess = 31,
  kWdgAliveNotification = 32,
  kFireWdgReaction = 33,
  kRecoveryHandlerFailed = 34
};

/// If IpcMessageType::ERROR, this is the sub type for details about the error
/// @brief IpcMessageErrorType class of enum type.
/// @design 613181
enum class IpcMessageErrorType : int32_t
{
  kBegin = 1,
  kNoType = 2,
  /// A publisher could not be created unique
  kNoUniqueCreated = 3,
  kInternalServiceDescriptionIsForbidden = 4,
  kRequestPublisherIinvalidResponse = 5,
  kRequestPublisherWrongIpcMessageResponse = 6,
  kRequestPublisherNoWritableShmSegment = 7,
  kRequestSubscriberInvalidResponse = 8,
  kRequestSubscriberWrongIpcMessageResponse = 9,
  kRequestClientInvalidResponse = 10,
  kRequestClientWrongIpcMessageResponse = 11,
  kRequestClientNoWritableShmSegment = 12,
  kRequestServerInvalidResponse = 13,
  kRequestServerWrongIpcMessageResponse = 14,
  kRequestServerNoWritableShmSegment = 15,
  kRequestConditionVariableInvalidResponse = 16,
  kRequestConditionVariableWrongIpcMessageResponse = 17,
  kPublisherListFull = 18,
  kSubscriberListFull = 19,
  kClientListFull = 20,
  kServerListFull = 21,
  kConditionVariableListFull = 22,
  kEventVariableListFull = 23,
  kNodeDataListFull = 24,
  kEnd = 25,
};

/// @brief Converts a string to the message type enumeration.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] str A string to convert.
/// @returns Reference to this string.
/// @archtrace 608897
/// @design 613182
template <typename T>
T StringToIpcMessageType(const char* str) noexcept;

/// @brief Converts a message type enumeration value into a string.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] msg A enum value to convert.
/// @returns Reference to the string.
/// @archtrace 608897
/// @design 613180
template <typename T>
std::string IpcMessageTypeToString(const T msg) noexcept;

/// @brief Converts a string to the message error type enumeration.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] str A string to convert.
/// @returns Reference to this string.
/// @archtrace 608897
/// @design 613184
template <typename T>
T StringToIpcMessageErrorType(const char* str) noexcept;

/// @brief Converts a message error type enumeration value into a string.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] msg A enum value to convert.
/// @returns Reference to the string.
/// @archtrace 608897
/// @design 613185
template <typename T>
std::string IpcMessageErrorTypeToString(const T msg) noexcept;

/// @brief Converts a string to the message type enumeration.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] str A string to convert.
/// @returns Reference to this string.
/// @archtrace 608897
/// @design 613186
template <typename T>
T StringToIpcMessageType(const char* str) noexcept
{
  typename std::underlying_type<T>::type msg;
  bool noError{iox::cxx::convert::stringIsNumber(str, iox::cxx::convert::NumberType::INTEGER)};
  noError &= noError ? (iox::cxx::convert::fromString(str, msg)) : false;
  noError &= noError ? !(static_cast<typename std::underlying_type<T>::type>(T::kBegin) >= msg
                           || static_cast<typename std::underlying_type<T>::type>(T::kEnd) <= msg)
                     : false;
  return noError ? (static_cast<T>(msg)) : T::kNoType;
}

/// @brief Define a IpcMessageTypeToString method.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] msg A enum value to convert.
/// @returns Reference to the string.
/// @archtrace 608897
/// @design 613187
template <typename T>
std::string IpcMessageTypeToString(const T msg) noexcept
{
  return iox::cxx::convert::toString(static_cast<typename std::underlying_type<T>::type>(msg));
}

/// @brief Converts a string to the message error type enumeration.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] str A string to convert.
/// @returns Reference to this string.
/// @archtrace 608897
/// @design 613188
template <typename T>
T StringToIpcMessageErrorType(const char* str) noexcept
{
  typename std::underlying_type<T>::type msg;
  bool noError{iox::cxx::convert::stringIsNumber(str, iox::cxx::convert::NumberType::INTEGER)};
  noError &= noError ? (iox::cxx::convert::fromString(str, msg)) : false;
  noError &= noError ? !(static_cast<typename std::underlying_type<T>::type>(T::kBegin) >= msg
                           || static_cast<typename std::underlying_type<T>::type>(T::kEnd) <= msg)
                     : false;
  return noError ? (static_cast<T>(msg)) : T::kNoType;
}

/// @brief Converts a ipc meaage error into string type enumeration.
/// @tparam T Datatype which is convertible to string via std::stringstream.
/// @param[in] msg A enum value to convert.
/// @returns Reference to the string.
/// @archtrace 608897
/// @design 613189
template <typename T>
std::string IpcMessageErrorTypeToString(const T msg) noexcept
{
  return iox::cxx::convert::toString(static_cast<typename std::underlying_type<T>::type>(msg));
}

} // namespace ipc
} // namespace core
} // namespace ara

#endif // UTILS_IPC_INC_IPC_MESSAGE_TYPES_HPP
