// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 by Apex.AI Inc. All rights reserved.
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
#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FIXED_VECTOR_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FIXED_VECTOR_H_

#include "iceoryx_hoofs/cxx/requires.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <utility>

namespace ara
{
namespace core
{
/// @brief  C++11 compatible FixedVector implementation.
/// We needed to do some adjustments in the API since we do not use exceptions
/// and we require a data structure which can be located fully in the shared memory.
/// @tparam T The type of contained values.
/// @tparam Capacity The capacity of the contained values.
/// @attention Out of bounds access or accessing an empty FixedVector can lead to a program termination!
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611255
template <typename T, uint64_t Capacity>
class FixedVector
{
 public:
  using value_type = T;

  using iterator = T*;
  using const_iterator = const T*;

  /// @brief It creates an empty FixedVector.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  FixedVector() noexcept = default;

  /// @brief It creates a FixedVector with count copies of elements with value.
  /// @param[in] count Is the number copies which are inserted into the FixedVector.
  /// @param[in] value Is the value which is inserted into the FixedVector.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611256
  FixedVector(const uint64_t count, const T& value) noexcept;

  /// @brief It creates a FixedVector with count copies of elements constructed
  /// with the default constructor of T.
  /// @param[in] count Is the number copies which are inserted into the FixedVector.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611257
  FixedVector(const uint64_t count) noexcept;

  /// @brief A copy constructor to copy a FixedVector of the same capacity.
  /// @param[in] rhs The right-hand side of the comparison.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611258
  FixedVector(const FixedVector& rhs) noexcept;

  /// @brief A move constructor to move a FixedVector of the same capacity.
  /// @param[in] rhs The right-hand side of the comparison.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611259
  FixedVector(FixedVector&& rhs) noexcept;

  /// @brief It destructs the FixedVector and also calls the destructor of all contained elements.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611260
  ~FixedVector() noexcept;

  /// @brief The copy assignment.
  /// If the destination FixedVector contains more elements
  /// than the source the remaining elements will be destructed.
  /// @param[in] rhs The right-hand side of the comparison.
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611261
  FixedVector& operator=(const FixedVector& rhs) noexcept;

  /// @brief The move assignment.
  /// If the destination FixedVector contains more elements
  /// than the source the remaining elements will be destructed.
  /// @param[in] rhs The right-hand side of the comparison.
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611262
  FixedVector& operator=(FixedVector&& rhs) noexcept;

  /// @brief It returns an iterator to the first element of the FixedVector,
  /// if the FixedVector is empty it returns the same iterator as end
  /// (the first iterator which is outside of the FixedVector).
  /// @returns Returns an iterator pointing to the first element in the sequence.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611263
  iterator begin() noexcept;

  /// @brief It returns a const iterator to the first element of the FixedVector,
  /// if the FixedVector is empty it returns the same iterator as end
  /// (the first iterator which is outside of the FixedVector).
  /// @returns Returns a const_iterator pointing to the first element in the sequence.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611264
  const_iterator begin() const noexcept;

  /// @brief It returns an iterator to the element which comes after the
  /// last element (the first element which is outside of the FixedVector).
  /// @returns Returns an iterator pointing to the last element in the sequence.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611265
  iterator end() noexcept;

  /// @brief It returns a const iterator to the element which comes after
  /// the last element (the first element which is outside of the FixedVector).
  /// @returns Returns a const_iterator pointing to the past-the-end element in the sequence.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611266
  const_iterator end() const noexcept;

  /// @brief It return the pointer to the underlying array.
  /// @returns A pointer to underlying array.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611267
  T* data() noexcept;

  /// @brief It return the const pointer to the underlying array.
  /// @returns A const pointer to underlying array.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611268
  const T* data() const noexcept;

  /// @brief It returns a reference to the element stored at index.
  /// The behavior is undefined if the element at index does not exist.
  /// @param[in] index The index of the type which should be constructed.
  /// @attention Out of bounds access can lead to a program termination!
  /// @returns The reference to the index.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611269
  T& at(const uint64_t index) noexcept;

  /// @brief It returns a const reference to the element stored at index.
  /// The behavior is undefined if the element at index does not exist.
  /// @param[in] index The index of the type which should be constructed.
  /// @attention Out of bounds access can lead to a program termination!
  /// @returns The const reference to the index.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611270
  const T& at(const uint64_t index) const noexcept;

  /// @brief It returns a reference to the element stored at index.
  /// The behavior is undefined if the element at index does not exist.
  /// @param[in] index The index of the type which should be constructed.
  /// @attention Out of bounds access can lead to a program termination!
  /// @returns Returns a reference to the element stored at index.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611271
  T& operator[](const uint64_t index) noexcept;

  /// @brief It returns a const reference to the element stored at index.
  /// The behavior is undefined if the element at index does not exist.
  /// @param[in] index The index of the type which should be constructed.
  /// @attention Out of bounds access can lead to a program termination!
  /// @returns Returns a const reference to the element stored at index.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611272
  const T& operator[](const uint64_t index) const noexcept;

  /// @brief It returns a reference to the first element; terminates if the FixedVector is empty.
  /// @returns It reference to the first element.
  /// @attention Accessing an empty FixedVector can lead to a program termination!
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611273
  T& front() noexcept;

  /// @brief It returns a const reference to the first element; terminates if the FixedVector is empty.
  /// @returns A const reference to the first element.
  /// @attention Accessing an empty FixedVector can lead to a program termination!
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611274
  const T& front() const noexcept;

  /// @brief It returns a reference to the last element; terminates if the FixedVector is empty.
  /// @returns A reference to the last element.
  /// @attention Accessing an empty FixedVector can lead to a program termination!
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611275
  T& back() noexcept;

  /// @brief It returns a const reference to the last element; terminates if the FixedVector is empty.
  /// @returns A const reference to the last element.
  /// @attention Accessing an empty FixedVector can lead to a program termination!
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611276
  const T& back() const noexcept;

  /// @brief It returns the capacity of the FixedVector which was given via the template argument.
  /// @returns Capacity of the currently allocated storage.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611277
  uint64_t capacity() const noexcept;

  /// @brief It returns the number of elements which are currently stored in the FixedVector.
  /// @returns The number of elements in the container.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611278
  uint64_t size() const noexcept;

  /// @brief It returns true if the FixedVector is emtpy, otherwise false.
  /// @returns True if the container is empty, false otherwise.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611279
  bool empty() const noexcept;

  /// @brief It calls the destructor of all contained elements and removes them.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611280
  void clear() noexcept;

  /// @brief It resizes the FixedVector.
  /// If the FixedVector size increases new elements will be constructed with the given
  /// arguments. If count is greater than the capacity the FixedVector will stay unchanged.
  /// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
  /// @param[in] count New size of the FixedVector.
  /// @param[in] args The arguments used for constructing the value.
  /// @returns True if the resize was successful, false if count is greater than the capacity.
  /// @note Perfect forwarded arguments are explicitly not wanted here.
  /// Think of what happens if resize creates two new elements via move construction.
  /// The first one has a valid source but the second gets an already moved parameter.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611281
  template <typename... Targs>
  bool resize(const uint64_t count, const Targs&... args) noexcept;

  /// @brief Forwards all arguments to the constructor of the contained
  /// element and performs a placement new at the provided position.
  /// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
  /// @param[in] position The position where the element should be created.
  /// @param[in] args A arguments which will be perfectly forwarded to the constructor of T.
  /// @returns An element that points to the newly emplaced position.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611282
  template <typename... Targs>
  bool emplace(const uint64_t position, Targs&&... args) noexcept;

  /// @brief Forwards all arguments to the constructor of the
  /// contained element and performs a placement new at the end.
  /// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
  /// @param[in] args A arguments which will be perfectly forwarded to the constructor of T.
  /// @returns If a reallocation happens, the storage is allocated using the container's
  /// allocator, which may throw exceptions on failure.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611283
  template <typename... Targs>
  bool emplace_back(Targs&&... args) noexcept;

  /// @brief It appends the given element at the end of the FixedVector.
  /// @param[in] value The value of the element to append.
  /// @returns True if successful, false if FixedVector already full.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611284
  bool push_back(const T& value) noexcept;

  /// @brief It appends the given element at the end of the FixedVector.
  /// @param[in] value The value of the element to append.
  /// @returns True if successful, false if FixedVector already full.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611285
  bool push_back(T&& value) noexcept;

  /// @brief It removes the last element of the FixedVector;
  /// calling pop_back on an empty container does nothing.
  /// @returns True if the last element was removed. If the FixedVector is empty it returns false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611286
  bool pop_back() noexcept;

  /// @brief It removes an element at the given position.
  /// If this element is in the middle of the FixedVector every element is moved
  /// one place to the left to ensure that the elements are stored contiguously.
  /// @param[in] position Iterator to the character to remove.
  /// @returns An iterator pointing to the new location of the element that followed
  /// the last element erased by the function call.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609887
  /// @design 611287
  iterator erase(iterator position) noexcept;

  uint64_t m_size = 0u;

 private:
  using element_t = uint8_t[sizeof(T)];
  alignas(T) element_t m_data[Capacity];
};
/// @brief Definition to support lexicographical compare.
/// @tparam InputIterator1 The first iterator.
/// @tparam InputIterator2 The second iterator.
/// @param[in] first1 Input iterator to initial position of first sequence.
/// @param[in] last1 Input iterator to final position of first sequence.
/// @param[in] first2 Input iterator to initial position of second sequence.
/// @param[in] last2 Input iterator to final position of second sequence.
/// @returns Returns a boolean true, if range1 is strictly lexicographically
/// smaller than range2 else returns a false.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611837
template <typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(
    InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
  while (first1 != last1)
  {
    if (first2 == last2 || *first2 < *first1)
      return false;
    else if (*first1 < *first2)
      return true;
    ++first1;
    ++first2;
  }
  return (first2 != last2);
}

/// @brief Definition to support the less than operator overloading.
/// @tparam T The type of the value.
/// @tparam CapacityLeft The capacity of the FixedVector.
/// @tparam CapacityRight The capacity of the FixedVector.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns Returns true if the first operand is smaller than the second operand.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611838
template <typename T, uint64_t CapacityLeft, uint64_t CapacityRight>
bool operator<(const FixedVector<T, CapacityLeft>& lhs, const FixedVector<T, CapacityRight>& rhs) noexcept
{
  return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/// @brief Definition to support the greater than operator overloading.
/// @tparam T The type of the value.
/// @tparam CapacityLeft The capacity of the FixedVector.
/// @tparam CapacityRight The capacity of the FixedVector.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns Returns true if the first operand is greater than the second operand.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611839
template <typename T, uint64_t CapacityLeft, uint64_t CapacityRight>
bool operator>(const FixedVector<T, CapacityLeft>& lhs, const FixedVector<T, CapacityRight>& rhs) noexcept
{
  return rhs < lhs;
}

/// @brief Definition to support the greater than or equal to operator overloading.
/// @tparam T The type of the value.
/// @tparam CapacityLeft The capacity of the FixedVector.
/// @tparam CapacityRight The capacity of the FixedVector.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns Returns true if the first operand is greater or equal than the second operand.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611840
template <typename T, uint64_t CapacityLeft, uint64_t CapacityRight>
bool operator>=(const FixedVector<T, CapacityLeft>& lhs, const FixedVector<T, CapacityRight>& rhs) noexcept
{
  return !(lhs < rhs);
}
/// @brief Definition to support the less than or equal to operator overloading.
/// @tparam T The type of the value.
/// @tparam CapacityLeft The capacity of the FixedVector.
/// @tparam CapacityRight The capacity of the FixedVector.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns Returns true if the first operand is smaller or equal than the second operand.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611841
template <typename T, uint64_t CapacityLeft, uint64_t CapacityRight>
bool operator<=(const FixedVector<T, CapacityLeft>& lhs, const FixedVector<T, CapacityRight>& rhs) noexcept
{
  return !(rhs < lhs);
}

} // namespace core
} // namespace ara

/// @brief Definition to support the equal to operator overloading.
/// @tparam T The type of the value.
/// @tparam CapacityLeft The capacity of the FixedVector.
/// @tparam CapacityRight The capacity of the FixedVector.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns Returns true if the first operand is equal to the second operand.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611837
template <typename T, uint64_t CapacityLeft, uint64_t CapacityRight>
bool operator==(const ara::core::FixedVector<T, CapacityLeft>& lhs,
    const ara::core::FixedVector<T, CapacityRight>& rhs) noexcept;

/// @brief Definition to support the not equal to operator overloading.
/// @tparam T The type of the value.
/// @tparam CapacityLeft The capacity of the FixedVector.
/// @tparam CapacityRight The capacity of the FixedVector.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns Returns true if the first operand is not equal to the second operand.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609887
/// @design 611837
template <typename T, uint64_t CapacityLeft, uint64_t CapacityRight>
bool operator!=(const ara::core::FixedVector<T, CapacityLeft>& lhs,
    const ara::core::FixedVector<T, CapacityRight>& rhs) noexcept;

#include "ara/core/fixed_vector.inl"

#endif // FIXED_VECTOR_H
