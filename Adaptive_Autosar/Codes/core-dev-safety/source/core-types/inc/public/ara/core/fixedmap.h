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
#pragma once

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FIXEDMAP_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FIXEDMAP_H_

#include "ara/core/fixed_vector.h"
using namespace std;
namespace ara
{
namespace core
{
/// @brief Maps are associative containers that store elements in a mapped way.
/// Each element has a key value and a mapped value. No two mapped values can have the same key values.
/// @tparam Key The object to be hashed.
/// @tparam T The mapped tpye element.
/// @tparam Capacity The size of the storage space currently allocated for the string.
/// @archtrace 609888
/// @design 611484 614039
template <class Key, class T, uint64_t Capacity = 100>
class FixedMap
{
 public:
  typedef Key key_type;
  typedef T mapped_type;
  typedef std::pair<Key, T> value_type;

  using iterator = value_type*;

  ara::core::FixedVector<value_type, 10> data;

  /// @brief Return the iterator to the first key.
  /// @returns Returns an iterator pointing to the first element in the key.
  /// @archtrace 609888
  /// @design 611485
  iterator begin()
  {
    return data.begin();
  }

  /// @brief Return the iterator to the end key.
  /// @returns Returns an iterator pointing to the end element in the key.
  /// @archtrace 609888
  /// @design 611486
  iterator end()
  {
    iterator it = data.end();
    return it;
  }

  /// @brief Return the std::pair to the matched key.
  /// @param[in] key A Key value of the element whose mapped value is accessed.
  /// @returns Returns an vector pointing to the first element in the key.
  /// @archtrace 609888
  /// @design 611487
  value_type& operator[](const Key& key)
  {
    for (auto& vec : data)
    {
      if (key == vec.first)
      {
        return vec;
      }
    }
  }

  /// @brief Return the iterator to the key if found in the vector.
  /// @param[in] key The object to be hashed.
  /// @returns Returns an iterator pointing to the end element in the key.
  /// @archtrace 609888
  /// @design 611488
  iterator find(const Key& key)
  {
    for (auto it = data.begin(); it != data.end(); it++)
    {
      if (key == it->first)
      {
        // if found, return the iterator to the vector
        return it;
      }
      else
      {
        // If key not found, it will return the end address of the vector

        auto it = data.end();
      }
    }
    return data.end();
  }

  /// @brief Return the iterator if the key is found.
  /// @param[in] key A Key value of the element whose mapped value is accessed.
  /// @returns A reference to the mapped value of the element with a Key value equivalent to key.
  /// @archtrace 609888
  /// @design 611489
  iterator at(const Key& key)
  {
    for (auto it = data.begin(); it != data.end(); it++)
    {
      try
      {
        if (key == it->first)
        {
          return it;
        }
        else
        {
          throw std::out_of_range("I am an exception.");
          auto it = data.end();
        }
      }

      catch (std::out_of_range)
      {
      }
    }
  }

  /// @brief Erase the pair for the matched key.
  /// @param[in] key A Key value of the element whose mapped value is accessed.
  /// @returns An iterator pointing to the new location of the element
  /// that followed the last element erased by the function call.
  /// @archtrace 609888
  /// @design 611490
  uint8_t erase(const Key& key)
  {
    uint8_t returnValue = 0;
    for (auto it = data.begin(); it != data.end(); it++)
    {
      //return 1 if the element found else return 0
      if (key == it->first)
      {
        data.erase(it);
        returnValue = 1;
        return returnValue;
      }
      else
      {
        returnValue = 0;
      }
    }
    return returnValue;
  }

  /// @brief Insert the pair into the vector.
  /// @param[in] value Value to be copied(or moved) to the new element.
  /// @archtrace 609888
  /// @design 611491
  void insert(std::pair<Key, T> value)
  {
    data.push_back(value);
  }

  /// @brief Return the size of the vector.
  /// @returns The number of elements in the vector.
  /// @archtrace 609888
  /// @design 611492
  uint64_t size()
  {
    return data.size();
  }

  /// @brief Return the size of the value for the matched key.
  /// @param[in] key A Key value of the element whose mapped value is accessed.
  /// @returns a key representing the hash value.
  /// @archtrace 609888
  /// @design 611493
  uint64_t valueSize(const Key& key)
  {
    for (value_type it : data)
    {
      //return 1 if the element found else return 0
      if (key == it.first)
      {
        auto value = it.second;
        return value.size();
      }
      else
      {
        return 0;
      }
    }
  }

  /// @brief All the elements of the pair are removed (or destroyed).
  /// @archtrace 609888
  /// @design 611494
  void clear()
  {
    data.clear();
  }

  /// @brief Check the pair is empty.
  /// @returns true if the pair is empty, false otherwise.
  /// @archtrace 609888
  /// @design 611495
  bool empty()
  {
    if (size() == 0)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
};

} // namespace core

} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FIXEDMAP_H_
