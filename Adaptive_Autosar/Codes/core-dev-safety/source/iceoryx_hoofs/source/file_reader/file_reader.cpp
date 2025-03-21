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

#include "iceoryx_hoofs/internal/file_reader/file_reader.hpp"
#include "iceoryx_hoofs/cxx/helplets.hpp"

#include <iostream>
#include <string>

namespace iox
{
namespace cxx
{
constexpr char FileReader::PATH_SEPARATOR[];

FileReader::FileReader(
    const std::string& f_fileName, const std::string& f_filePath, ErrorMode f_errorMode) noexcept :
    m_errorMode{f_errorMode}
{
  m_file = f_filePath.empty() ? f_fileName : f_filePath + PATH_SEPARATOR + f_fileName;
  m_fileStream.open(m_file, std::fstream::in);

  if (!isOpen())
  {
    switch (m_errorMode)
    {
    case ErrorMode::Ignore:
    {
      return;
    }
    default:
    case ErrorMode::Inform:
    {
      std::cerr << "\033[5;31m"
                << "Could not open file '" << m_file << "'."
                << "\033[0m" << std::endl;
      return;
    }
    case ErrorMode::Terminate:
    {
      std::cerr << "\033[5;31m"
                << "Could not open file '" << m_file << "'. Exiting!"
                << "\033[0m" << std::endl;
      cxx::Ensures(false);
      return;
    }
    }
  }
}
bool FileReader::isOpen() const noexcept
{
  return m_fileStream.is_open();
}

bool FileReader::readLine(std::string& f_string) noexcept
{
  return static_cast<bool>(std::getline(m_fileStream, f_string));
}

} // namespace cxx
} // namespace iox
