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

#include "ipc_message.hpp"

#include <algorithm>
namespace ara
{
namespace core
{
namespace ipc
{
const char IpcMessage::stkSeparator_{','};

IpcMessage::IpcMessage(const std::initializer_list<std::string>& msg) noexcept
{
  for (auto element : msg)
  {
    AddEntry(element);
  }
}

IpcMessage::IpcMessage(const std::string& msg) noexcept
{
  SetMessage(msg);
}

uint32_t IpcMessage::GetNumberOfElements() const noexcept
{
  return mNumberOfElements_;
}

std::string IpcMessage::GetElementAtIndex(const uint32_t index) const noexcept
{
  std::string messageRemainder{mMsg_};
  size_t startPos{0u};
  size_t endPos{messageRemainder.find_first_of(stkSeparator_, startPos)};

  for (uint32_t counter{0u}; endPos != std::string::npos; ++counter)
  {
    if (counter == index)
    {
      return messageRemainder.substr(startPos, endPos - startPos);
    }

    startPos = endPos + 1u;
    endPos = messageRemainder.find_first_of(stkSeparator_, startPos);
  }

  return std::string();
}

bool IpcMessage::IsValidEntry(const std::string& entry) const noexcept
{
  if (entry.find(stkSeparator_) != std::string::npos)
  {
    return false;
  }
  return true;
}

bool IpcMessage::IsValid() const noexcept
{
  //return mIsValid_;

  return true;
}

std::string IpcMessage::GetMessage() const noexcept
{
  return mMsg_;
}

void IpcMessage::SetMessage(const std::string& msg) noexcept
{
  ClearMessage();

  mMsg_ = msg;
  if (!mMsg_.empty() && mMsg_.back() != stkSeparator_)
  {
    mIsValid_ = false;
  }
  else
  {
    mNumberOfElements_ = static_cast<uint32_t>(
        std::count_if(mMsg_.begin(), mMsg_.end(), [&](char c) { return c == stkSeparator_; }));
  }
}

void IpcMessage::ClearMessage() noexcept
{
  mMsg_.clear();
  mNumberOfElements_ = 0u;
  mIsValid_ = true;
}

bool IpcMessage::operator==(const IpcMessage& rhs) const noexcept
{
  return this->GetMessage() == rhs.GetMessage();
}

} // namespace ipc
} // namespace core
} // namespace ara