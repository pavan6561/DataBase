// Copyright (c) Giacomo Drago <giacomo@giacomodrago.com>
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of Giacomo Drago nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY GIACOMO DRAGO "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GIACOMO DRAGO BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef CORE_SOURCE_UTILS_CONFIG_READER_INC_PUBLIC_ARA_CORE_CONFIGREADER_CONFIG_READER_H_
#define CORE_SOURCE_UTILS_CONFIG_READER_INC_PUBLIC_ARA_CORE_CONFIGREADER_CONFIG_READER_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <forward_list>
#include <functional>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "invoke.h"
#include "ara/core/optional.h"
#include "ara/core/string_view.h"
#include "iceoryx_hoofs/cxx/convert.hpp"

using namespace std;

#ifndef MJR_NESTING_LIMIT
#define MJR_NESTING_LIMIT 32
#endif

#define MJR_STRINGIFY(S) MJR_STRINGIFY_HELPER(S)
#define MJR_STRINGIFY_HELPER(S) #S
namespace detail
{
/// @brief Implementation is used to Invoke the Callable object f with a tuple.
/// of arguments and pack of parameters.
/// @tparam F Variable is the template parameter of class type.
/// @tparam Tuple Variable is the template parameter of class type.
/// @tparam I Variable is the template parameter which will take pack of parameters.
/// @param[in] f Callable object to be invoked
/// @param[in] t Tuple whose elements to be used as arguments to f
/// @param[in] x Variable is instance of std::index_sequence.
/// @returns invoke.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611144
template <class F, class Tuple, std::size_t... I>
constexpr decltype(auto) ApplyImpl(F&& f, Tuple&& t, std::index_sequence<I...> x)
{
  // This implementation is valid since C++20 (via P1065R2)
  // In C++17, a constexpr counterpart of std::invoke is actually needed here
  return eggs::Invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

} // namespace detail

namespace app
{
/// @brief Implementation is used to invoke the Callable object f with a tuple of arguments.
/// @tparam F Variable is callable object to be invoked.
/// @tparam Tuple Whose elements to be used as arguments to f
/// @param[in] f Variable is a the instance of  F.
/// @param[in] t Variable is a instance of Tuple.
/// @returns ApplyImpl.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611165
template <class F, class Tuple>
constexpr decltype(auto) Apply(F&& f, Tuple&& t)
{
  return detail::ApplyImpl(std::forward<F>(f), std::forward<Tuple>(t),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}
} // namespace app
//

namespace ara
{
namespace core
{
namespace configreader
{
namespace detail
{
/// @brief Definition of class ContextBase. Base for all context classes.
/// @archtrace 608895
/// @design 611069 614016
class ContextBase
{
 public:
  /// @brief Copy construction shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ContextBase(const ContextBase&) = delete;

  /// @brief Move constructor for the class ContextBase.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ContextBase(ContextBase&&) noexcept = default;

  /// @brief Copy assignment operator shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ContextBase& operator=(const ContextBase&) = delete;

  /// @brief Move assignment operator of the class ContextBase.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ContextBase& operator=(ContextBase&&) & noexcept = default;

  /// @brief The destructor for the class ContextBase.
  virtual ~ContextBase() noexcept = default;

  /// @brief An enumeration with ContextNestedStatus that can occur within this Functional Cluster.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  enum ContextNestedStatus
  {
    kNestedStatusNone,
    kNestedStatusObject,
    kNestedStatusArray
  };

  /// @brief Return a mNestedStatus_ to ContextNestedStatus of enum type.
  /// @returns It returns nested status.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611070
  ContextNestedStatus NestedStatus() const noexcept
  {
    return mNestedStatus_;
  }

  /// @brief Definition of the function BeginNested, used to capture the mNestedStatus_
  /// and increment mNestingLevel_ by 1.
  /// @param[in] nestedStatus Variable is instance of ContextNestedStatus.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611071
  void BeginNested(const ContextNestedStatus nestedStatus) noexcept
  {
    mNestedStatus_ = nestedStatus;
    ++mNestingLevel_;
  }

  /// @brief Method is used to reset the ContextNestedStatus to kNestedStatusNone.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611072
  void ResetNestedStatus() noexcept
  {
    mNestedStatus_ = kNestedStatusNone;
  }

  /// @brief Method is used to decrement the mNestingLevel_ by 1.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611073
  void EndNested() noexcept
  {
    if (mNestingLevel_ > 0)
    {
      --mNestingLevel_;
    }
  }

  /// @brief Implementation is used to return NestingLevel.
  /// @returns It returns sizeof nesting level.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611074
  std::size_t NestingLevel() const noexcept
  {
    return mNestingLevel_;
  }

 protected:
  /// @brief It is default constructor of the class.
  explicit ContextBase() noexcept = default;

 private:
  /// @brief To store the nested status.
  ContextNestedStatus mNestedStatus_{kNestedStatusNone};

  /// @brief To store the nesting level.
  std::size_t mNestingLevel_{0};
}; // class ContextBase

/// @brief Encapsulation of BufferContextBase. Base for context classes backed by a buffer.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611106
class BufferContextBase : public ContextBase
{
 public:
  /// @brief Copy construction shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  BufferContextBase(const BufferContextBase&) = delete;

  /// @brief Move constructor for the class BufferContextBase.
  /// @param[in] other Variable is of type Other object data.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611107
  BufferContextBase(BufferContextBase&& other) noexcept :
      ContextBase(std::move(other)), mReadBuffer_(other.mReadBuffer_), mWriteBuffer_(other.mWriteBuffer_),
      mLength_(other.mLength_), mReadOffset_(other.mReadOffset_), mWriteOffset_(other.mWriteOffset_),
      mCurrentLiteral_(other.mCurrentLiteral_)
  {
    other.mReadBuffer_ = nullptr;
    other.mWriteBuffer_ = nullptr;
    other.mLength_ = 0;
    other.mReadOffset_ = 0;
    other.mWriteOffset_ = 0;
    other.mCurrentLiteral_ = nullptr;
  }

  /// @brief Copy assignment operator shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  BufferContextBase& operator=(const BufferContextBase&) = delete;

  /// @brief Move assignable is supported for buffer context base.
  /// @param[in] other Variable is of Other object data.
  /// @returns It returns the reference of BufferContextBase.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611108
  BufferContextBase& operator=(BufferContextBase&& other) & noexcept
  {
    if (this != &other)
    {
      auto moved{std::move(other)};
      swap(moved);
    }
    return *this;
  }
  /// @brief The destructor for the class ContextBase.
  virtual ~BufferContextBase() noexcept = default;
  /// @brief Method returns the read buffer.
  /// @returns Method Read() returns char value.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611109
  char Read() noexcept
  {
    if (mReadOffset_ >= mLength_)
    {
      return 0;
    }

    return mReadBuffer_[mReadOffset_++];
  }

  /// @brief Implementation used to return the read offset.
  /// byte offset from the beginning of a standard-layout type to specified member
  /// @returns Method returns size of read offset.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611110
  std::size_t ReadOffset() const noexcept
  {
    return mReadOffset_;
  }

  /// @brief Method used to capture current literal.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611111
  void BeginLiteral() noexcept
  {
    mCurrentLiteral_ = mWriteBuffer_ + mWriteOffset_;
  }

  /// @brief Method first will check whether the write offset is greater than or equal to read offset,
  /// then memory may have serious bug or memory is corrupted and operation will get aborted.
  /// The data shall be initialised in write buffer.
  /// @param[in] c Passing the constant character.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611112
  void Write(const char c) noexcept
  {
    // LCOV_EXCL_START
    if (mWriteOffset_ >= mReadOffset_)
    {
      // This is VERY bad.
      // If we reach this line, then either the library contains a most
      // serious bug, or the memory is hopelessly corrupted. Better to
      // fail fast and get a crash dump. If this happens and you can
      // prove it's not the client's fault, please do file a bug report.
      std::abort();
    }
    // LCOV_EXCL_STOP

    mWriteBuffer_[mWriteOffset_++] = c;
  }

  /// @brief Implementation used to returns a current literal.
  /// @returns Method will return pointer of char value of current literal.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611113
  const char* CurrentLiteral() const noexcept
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    return mCurrentLiteral_;
  }

  /// @brief Implementation is used to get a current literal length.
  /// @returns Method returns size of current literal length.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611114
  std::size_t CurrentLiteralLength() const noexcept
  {
    return mWriteBuffer_ + mWriteOffset_ - mCurrentLiteral_;
  }

  /// @brief Implementation is used to Swap other object data with current object data.
  /// @param[in] other Variable is Other object data.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611115
  void swap(BufferContextBase& other) noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    using std::swap;
    swap(static_cast<ContextBase&>(*this), static_cast<ContextBase&>(other));
    swap(mReadBuffer_, other.mReadBuffer_);
    swap(mWriteBuffer_, other.mWriteBuffer_);
    swap(mLength_, other.mLength_);
    swap(mReadOffset_, other.mReadOffset_);
    swap(mWriteOffset_, other.mWriteOffset_);
    swap(mCurrentLiteral_, other.mCurrentLiteral_);
  }

  /// @brief Method used to Swap LHS and RHS object data.
  /// @param[in] lhs Variable is left hand side object data.
  /// @param[in] rhs Variable is right hand side object data.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  friend void swap(BufferContextBase& lhs, BufferContextBase& rhs) noexcept
  //AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
  {
    lhs.swap(rhs);
  }

 protected:
  /// @brief Implementation of Paramerterised constructor.
  /// @param[in] readBuffer Variable is char pointer type const variable.
  /// @param[in] writeBuffer Variable is char pointer type const variable.
  /// @param[in] length Variable is size_t type const variable.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611116
  explicit BufferContextBase(
      const char* const readBuffer, char* const writeBuffer, const std::size_t length) noexcept :
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
      mReadBuffer_(readBuffer),
      mWriteBuffer_(writeBuffer), mLength_(length)
  {
  }

  /// @brief Implementation is used to get the write buffer.
  /// @returns Method returns character pointer of write buffer.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611117
  char* writeBuffer() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mWriteBuffer_;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

 private:
  /// @brief Constant char pointer to store read buffer..
  const char* mReadBuffer_;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief A char pointer to store write buffer.
  char* mWriteBuffer_;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief To store the size of length.
  std::size_t mLength_;

  /// @brief To store the size of read offset.
  std::size_t mReadOffset_{0};

  /// @brief To store the size of write offset.
  std::size_t mWriteOffset_{0};

  /// @brief Constant char pointer to store current literal.
  const char* mCurrentLiteral_{mWriteBuffer_};
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

}; // class BufferContextBase

/// @brief Encapsulation of Utility class used throughout the library to read JSON literals (strings,
/// bools, nulls, numbers) from a context and write them back into the context
/// after applying the necessary transformations (e.g. escape sequences).
/// @tparam Context Variable is the template argument.
/// Utility class used throughout the library to read JSON literals (strings,
/// bools, nulls, numbers) from a context and write them back into the context
/// after applying the necessary transformations (e.g. escape sequences).
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611087
template <typename Context>
class LiteralIo final
{
 public:
  /// @brief Implementation of copy constructor for the class LiteralIo.
  /// @param[in] context Variable is of type Context.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611088
  explicit LiteralIo(Context& context) noexcept : mContext_(context)
  {
    mContext_.BeginLiteral();
  }

  /// @brief Implementationis used to return reference of context.
  /// @returns It returns the reference of context.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611089
  Context& context() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mContext_;
  }

  /// @brief Implementation is used to read the data.
  /// @returns It returns data of char type.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611090
  char Read() noexcept
  {
    return mContext_.Read();
  }

  /// @brief Implementation is used to write the data.
  /// @param[in] c Constant char passing as argument.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611091
  void Write(const char c) noexcept
  {
    mContext_.Write(c);
  }

  /// @brief Implementation is used to get cuurent literal and length.
  /// @returns It returns instance of StringView.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611092
  ara::core::StringView Finalize() noexcept
  {
    // Get the length of the literal
    const std::size_t length{mContext_.CurrentLiteralLength()};

    // Write a null terminator. This is not strictly required, but brings
    // some extra safety at negligible cost.
    mContext_.Write(0);

    return {mContext_.CurrentLiteral(), length};
  }

 private:
  /// @brief To store the reference of context.
  Context& mContext_;
}; // class LiteralIo

} // namespace detail

/// @brief Encapsulation of class BufferContext.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611137
class BufferContext final : public detail::BufferContextBase
{
 public:
  /// @brief Implementation of parameterised constructor.
  /// @param[in] buffer Constant variable which is of type char pointer.
  /// @param[in] length Variable is of type std::size_t which is constant.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611138
  explicit BufferContext(char* const buffer, const std::size_t length) noexcept :
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
      detail::BufferContextBase(buffer, buffer, length)
  {
  }

  /// @brief Copy constructor for class BufferContext shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  BufferContext(const BufferContext&) = delete;

  /// @brief Move constructor for class BufferContext.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  BufferContext(BufferContext&&) = default;

  /// @brief Copy assignment operator of class BufferContext shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  BufferContext& operator=(const BufferContext&) = delete;

  /// @brief Move assignment operator for the class BufferContext
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  BufferContext& operator=(BufferContext&&) & noexcept = default;
}; // class BufferContext

/// @brief Encapsulation of class constant_buffer_context.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611118
class ConstBufferContext final : public detail::BufferContextBase
{
 public:
  /// @brief Implementation of parameterised constructor.
  /// @param[in] buffer Variable is of type char pointer.
  /// @param[in] length Variable is of type std::size_t.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611119
  explicit ConstBufferContext(const char* const buffer, const std::size_t length) :
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
      detail::BufferContextBase(buffer, new char[length], length)
  // don't worry about leaks, BufferContextBase can't throw
  {
  }

  /// @brief Copy constructor for class ConstBufferContext shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ConstBufferContext(const ConstBufferContext&) = delete;

  /// @brief Move constructor for class ConstBufferContext.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ConstBufferContext(ConstBufferContext&&) noexcept = default;

  /// @brief Copy assignment operator shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ConstBufferContext& operator=(const ConstBufferContext&) noexcept = delete;

  /// @brief Move assignment operator for the class ConstBufferContext.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  ConstBufferContext& operator=(ConstBufferContext&&) & = default;

  /// @brief Implementation of destructor for class ConstBufferContext.
  /// It will free up the memory from writeBuffer.Destructor is an instance.
  /// member function which is invoked automatically whenever an object is
  /// going to be destroyed. Meaning, a destructor is the last function that
  /// is going to be called before an object is destroyed.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611120
  virtual ~ConstBufferContext() noexcept
  {
    delete[] writeBuffer();
  }
}; // class ConstBufferContext

/// @brief Encapsulation of class IstreamContext.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611118
class IstreamContext final : public detail::ContextBase
{
 public:
  /// @brief Implementation of parameterised constructor.
  /// This is the preferred method to initialize member data.
  /// @param[in] stream Variable is of type std::istream reference.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611098 611099
  explicit IstreamContext(std::istream& stream) : mStream_(&stream)
  {
  }

  /// @brief Copy constructor for class IstreamContext shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  IstreamContext(const IstreamContext&) = delete;

  /// @brief Move constructor for class IstreamContext.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  IstreamContext(IstreamContext&&) noexcept = default;

  /// @brief Copy assignment operator for class IstreamContext shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  IstreamContext& operator=(const IstreamContext&) = delete;

  /// @brief Move assignment operator for class IstreamContext.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  IstreamContext& operator=(IstreamContext&&) & noexcept = default;

  /// @brief In this method mStream_ will Call the get method and the data will get
  /// stored in variable, If there is data present in the mStream_ ,It will
  /// return data in char format, If daa is not present it will return 0.
  /// @returns It returns data in char type.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611100
  char Read()
  {
    const std::int32_t c{mStream_->get()};

    if (*mStream_)
    {
      ++mReadOffset_;

      return static_cast<char>(c);
    }
    else
    {
      return 0;
    }
  }

  /// @brief Implementation is used to get the read offset.
  /// @returns Method returns read offset of type std::size_t.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611101
  std::size_t ReadOffset() const noexcept
  {
    return mReadOffset_;
  }

  /// @brief This implementation  is used to insert new element in the container.
  /// This  new element is added to the beginning of the list.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611102
  void BeginLiteral()
  {
    mLiterals_.emplace_front();
  }

  /// @brief Implementation is used to Write data.
  /// @param[in] c A constant variable of the type char.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611103
  void Write(const char c)
  {
    mLiterals_.front().push_back(c);
  }

  /// @brief Implementation is used to retrieve the address of the current literal MUST be called
  /// AFTER all the calls to write() for the current current literal have been performed.
  /// @returns It returns a constant char pointer.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611104
  const char* CurrentLiteral() const noexcept
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    const std::vector<char>& literal{mLiterals_.front()};

    return !literal.empty() ? literal.data() : nullptr;
  }

  /// @brief Implementation of current_literal_length method.
  /// Method will capture first element and size of vector.
  /// @returns It returns first element and size of vector.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611105
  std::size_t CurrentLiteralLength() const noexcept
  {
    return mLiterals_.front().size();
  }

 private:
  /// @brief To store the inputs.
  std::istream* mStream_;

  /// @brief To store the offset.
  std::size_t mReadOffset_{0};

  /// @brief To store the vector.
  std::forward_list<std::vector<char>> mLiterals_;
}; // class IstreamContext

/// @brief Encapsulation of class ParseError, It contains enum ErrorReason.
/// @archtrace 608895 608954
/// @design 611060
class ParseError final : public std::exception
{
 public:
  /// @brief Following enum defines various error reasons.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  enum ErrorReason
  {
    kUnknown,
    kExpectedOpeningQuote,
    kExpectedUtf16LowSurrogate,
    kInvalidEscapeSequence,
    kInvalidUtf16Character,
    kInvalidValue,
    kUnterminatedValue,
    kExpectedOpeningBracket,
    kExpectedColon,
    kExpectedCommaOrClosingBracket,
    kNestedObjectOrArrayNotParsed,
    kExpectedNestingLimit,
    kNullUtf16Character,
    kExpectedValue,
    kUnescapedControlCharacter,
  };

  /// @brief Implementation is of parameterised constructor of the class ParseError.
  /// @tparam Context This is the template parameter.
  /// @param[in] context Constant Reference of context is used as parameter.
  /// @param[in] reason  This is type of ErrorReason which is constant.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895 608954
  /// @design 611061
  template <typename Context>
  explicit ParseError(const Context& context, const ErrorReason reason) noexcept :
      mOffset_(GetOffset(context)), mReason_(reason)
  {
  }

  /// @brief Implementation is used to get the offset.
  /// @returns It returns the offset of type std::size_t.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611062
  std::size_t Offset() const noexcept
  {
    return mOffset_;
  }

  /// @brief  Implementation is used to get the reason for error.
  /// @returns It returns the reason of type ErrorReason.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611063
  ErrorReason Reason() const noexcept
  {
    return mReason_;
  }

  /// @brief Implementation is used returns the reason for the error.
  /// This method checks the reason for the error.
  /// @returns It returns the constant char pointer.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611064
  const char* what() const noexcept override
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    const char* msg;
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
    switch (mReason_)
    {
    case kUnknown:
    {
      msg = "kUnknown parse error";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kExpectedOpeningQuote:
    {
      msg = "Expected opening quote";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kExpectedUtf16LowSurrogate:
    {
      msg = "Expected UTF-16 low surrogate";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kInvalidEscapeSequence:
    {
      msg = "Invalid escape sequence";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kInvalidUtf16Character:
    {
      msg = "Invalid UTF-16 character";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kInvalidValue:
    {
      msg = "Invalid value";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kUnterminatedValue:
    {
      msg = "Unterminated value";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kExpectedOpeningBracket:
    {
      msg = "Expected opening bracket";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kExpectedColon:
    {
      msg = "Expected colon";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kExpectedCommaOrClosingBracket:
    {
      msg = "Expected comma or closing bracket";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kNestedObjectOrArrayNotParsed:
    {
      msg = "Nested object or array not parsed";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kExpectedNestingLimit:
    {
      msg = "Exceeded nesting limit (" MJR_STRINGIFY(MJR_NESTING_LIMIT) ")";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kNullUtf16Character:
    {
      msg = "kNull UTF-16 character";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kExpectedValue:
    {
      msg = "Expected a value";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case kUnescapedControlCharacter:
    {
      msg = "Unescaped control character";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    default:
    {
      return ""; // to suppress compiler warnings -- LCOV_EXCL_LINE
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    }
    }
    return msg;
  }

 private:
  /// @brief Implementation is used to get the offset value.
  /// @tparam Context This is the template parameter.
  /// @param[in] context Variable is of type Context.
  /// @returns Method returns offset value of type std::size_t.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611061
  template <typename Context>
  static std::size_t GetOffset(const Context& context) noexcept
  {
    const std::size_t readOffset{context.ReadOffset()};

    return (readOffset != 0) ? (readOffset - 1) : 0;
  }

  /// @brief To store the offset of type std::size_t.
  std::size_t mOffset_;

  /// @brief To store the reason of type ErrorReason.
  ErrorReason mReason_;
}; // class ParseError

/// @brief The implementation of operator overloading of insertion operator to get reason for the error.
/// The class template basic_ostream provides support for high level output operations on character streams.
/// @param[in,out] out Variable is of type std ostream&
/// @param[in] reason Variable of type parse error::error reason.
/// @returns It returns the error reason to ostream(console.)
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611145
inline std::ostream& operator<<(std::ostream& out, const ParseError::ErrorReason reason)
{
  switch (reason)
  {
  case ParseError::kUnknown:
  {
    out << "kUnknown";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kExpectedOpeningQuote:
  {
    out << "kExpectedOpeningQuote";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kExpectedUtf16LowSurrogate:
  {
    out << "kExpectedUtf16LowSurrogate";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kInvalidEscapeSequence:
  {
    out << "kInvalidEscapeSequence";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kInvalidUtf16Character:
  {
    out << "kInvalidUtf16Character";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kInvalidValue:
  {
    out << "kInvalidValue";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kUnterminatedValue:
  {
    out << "kUnterminatedValue";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kExpectedOpeningBracket:
  {
    out << "kExpectedOpeningBracket";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kExpectedColon:
  {
    out << "kExpectedColon";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kExpectedCommaOrClosingBracket:
  {
    out << "kExpectedCommaOrClosingBracket";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kNestedObjectOrArrayNotParsed:
  {
    out << "kNestedObjectOrArrayNotParsed";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kExpectedNestingLimit:
  {
    out << "kExpectedNestingLimit";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kNullUtf16Character:
  {
    out << "kNullUtf16Character";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kExpectedValue:
  {
    out << "kExpectedValue";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  case ParseError::kUnescapedControlCharacter:
  {
    out << "kUnescapedControlCharacter";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  }
  default:
  {
    return out << "kUnknown";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  }
  return out;
}
// LCOV_EXCL_STOP

namespace detail
{
/// @brief The implementation checks for the whitespaces.
/// Tells whether a character is acceptable JSON whitespace to separate tokens
/// @param[in] c Variable is of type const char.
/// @returns It returns true if the whitespaces are present, it return false if there are no whitespaces.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611150
inline bool IsWhitespace(const char c)
{
  bool whitespace;
  switch (c)
  {
  case ' ':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      whitespace = true;
      break;
    }
  case '\n':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      whitespace = true;
      break;
    }
  case '\r':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      whitespace = true;
      break;
    }
  case '\t':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      whitespace = true;
      break;
    }
  default:
  {
    return false;
  }
  }
  return whitespace;
}

/// @brief The implementation tells whether a character can be used to terminate
/// a value not enclosed in quotes (i.e. kNull, kBoolean and kNumber).
/// @param[in] c Variable is of type const char.
/// @returns It returns true if input parameter is ('c','}',']'), else it will Call IsWhitespace().
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611151
inline bool IsValueTermination(const char c)
{
  bool isvalue;
  switch (c)
  {
  case ',':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      isvalue = true;
      break;
    }
  case '}':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      isvalue = true;
      break;
    }
  case ']':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      isvalue = true;
      break;
    }
  default:
  {
    return IsWhitespace(c);
  }
  }
  return isvalue;
}

/// @brief The implementation Tells whether a character used is digit or not.
/// There is an std::isdigit() but it's weird (takes an std::int32_t among other things)
/// @param[in] c Variable is of type const char.
/// @returns It returns true if input parameter is among the following cases, else it will return false.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611152
inline bool IsDigit(const char c)
{
  bool dig;
  switch (c)
  {
  case '0':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '1':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '2':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '3':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '4':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '5':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '6':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '7':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '8':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  case '9':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      dig = true;
      break;
    }
  default:
  {
    return false;
  }
  }
  return dig;
}

/// @brief The implementation Tells whether a character used is hex digit or not.
/// There is an std::isxdigit() but it's weird (takes an std::int32_t among other things)
/// @param[in] c Variable is of type const char.
/// @returns It returns true if input parameter is among the following cases or
/// it will Call IsDigit(), else it will return true.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611153
inline bool IsHexDigit(const char c)
{
  switch (c)
  {
  case 'a':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'A':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'b':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'B':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'c':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'C':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'd':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'D':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'e':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'E':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'f':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  case 'F':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    break;
  default:
  {
    return IsDigit(c);
  }
  }
  return true;
}

/// @brief Definition of struct EncodingError.
/// This exception is thrown internally by the functions dealing with UTF-16
/// escape sequences and is not propagated outside of the library
struct EncodingError
{
};

/// @brief Implementation is used to return 32 bit utf (Unicode Transformation Format) from 16 bit utf.
/// @param[in] high Variable is of type std::uint16_t.
/// @param[in] low Variable is of type std::uint16_t.
/// @returns It returns 32 bit utf of type std::uint32_t.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611154
inline std::uint32_t Utf16ToUtf32(std::uint16_t high, std::uint16_t low)
{
  std::uint32_t result;

  if ((high <= 0xD7FF) || (high >= 0xE000))
  {
    if (low != 0)
    {
      // Since the high code unit is not a surrogate, the low code unit
      // should be zero
      throw EncodingError();
    }

    result = high;
  }
  else
  {
    if (high > 0xDBFF) // we already know high >= 0xD800
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    {
      // The high surrogate is not within the expected range
      throw EncodingError();
    }

    if ((low < 0xDC00) || (low > 0xDFFF))
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    {
      // The low surrogate is not within the expected range
      throw EncodingError();
    }

    high -= 0xD800;
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    low -= 0xDC00;
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    result = 0x010000 + ((high << 10) | low);
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  }

  return result;
}

/// @brief Implementation is used to return 32 bit utf (Unicode Transformation Format) from 16 bit utf.
/// @param[in] utf32_char Variable is of type std::uint32_t
/// @returns It returns 8 bit utf of type std::uint8_t.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611155
inline std::array<std::uint8_t, 4> Utf32ToUtf8(const std::uint32_t utf32_char)
{
  std::array<std::uint8_t, 4> result{};

  // All the static_casts below are to please VS2022
  if (utf32_char <= 0x00007F)
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  {
    std::get<0>(result) = static_cast<std::uint8_t>(utf32_char);
  }
  else if (utf32_char <= 0x0007FF)
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  {
    std::get<0>(result) = static_cast<std::uint8_t>(0xC0 | ((utf32_char & (0x1F << 6)) >> 6));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    std::get<1>(result) = static_cast<std::uint8_t>(0x80 | (utf32_char & 0x3F));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  }
  else if (utf32_char <= 0x00FFFF)
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  {
    std::get<0>(result) = static_cast<std::uint8_t>(0xE0 | ((utf32_char & (0x0F << 12)) >> 12));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    std::get<1>(result) = static_cast<std::uint8_t>(0x80 | ((utf32_char & (0x3F << 6)) >> 6));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    std::get<2>(result) = static_cast<std::uint8_t>(0x80 | (utf32_char & 0x3F));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  }
  else if (utf32_char <= 0x1FFFFF)
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  {
    std::get<0>(result) = static_cast<std::uint8_t>(0xF0 | ((utf32_char & (0x07 << 18)) >> 18));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    std::get<1>(result) = static_cast<std::uint8_t>(0x80 | ((utf32_char & (0x3F << 12)) >> 12));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    std::get<2>(result) = static_cast<std::uint8_t>(0x80 | ((utf32_char & (0x3F << 6)) >> 6));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    std::get<3>(result) = static_cast<std::uint8_t>(0x80 | (utf32_char & 0x3F));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
  }
  else
  {
    // Invalid code unit
    throw EncodingError();
  }

  return result;
}

/// @brief Implementation is used to returns 8 bit utf (Unicode Transformation Format) from 16 bit utf.
/// @param[in] high Variable is of type std::uint16_t.
/// @param[in] low Variable is of type std::uint16_t.
/// @returns It returns 32 bit utf of type std::uint32_t.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611156
inline std::array<std::uint8_t, 4> Utf16ToUtf8(const std::uint16_t high, const std::uint16_t low)
{
  return Utf32ToUtf8(Utf16ToUtf32(high, low));
}

/// @brief The implementation gives hex value for the digits.
/// @param[in] c Variable is of type const char.
/// @returns It returns hex value for the input parameter of type std::uint8_t.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611157
inline std::uint8_t ParseHexDigit(const char c)
{
  std::uint8_t var{0};
  switch (c)
  {
  case '0':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x0;
      break;
    }
  case '1':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x1;
      break;
    }
  case '2':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x2;
      break;
    }
  case '3':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x3;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case '4':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x4;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case '5':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x5;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case '6':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x6;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case '7':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x7;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case '8':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x8;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case '9':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0x9;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'a':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xA;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'A':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xA;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'b':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xB;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'B':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xB;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'c':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xC;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'C':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xC;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'd':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xD;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'D':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xD;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'e':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xE;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'E':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xE;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'f':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xF;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  case 'F':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      var = 0xF;
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      break;
    }
  default:
    throw EncodingError();
  }
  return var;
}

/// @brief The implementation is used to return escape sequence.
/// @param[in] sequence Variable is of type array of char.
/// @returns It returns result of type std::uint16_t.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611158
inline std::uint16_t ParseUtf16EscapeSequence(const std::array<char, 4>& sequence)
{
  std::uint16_t result{0};

  for (const char c : sequence)
  {
    result <<= 4;
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
    result |= ParseHexDigit(c);
  }

  return result;
}

/// @brief The implementation is used to return escape sequence.
/// @tparam Context Variable is the type of datatype of parameter.
/// @param[in] LiteralIo Variable is of type reference of LiteralIo<Context>.
/// @param[in] c Variable is of type constant integer array(std::array<std::uint8_t).
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611159
template <typename Context>
void WriteUtf8Char(LiteralIo<Context>& LiteralIo, const std::array<std::uint8_t, 4>& c)
{
  LiteralIo.Write(std::get<0>(c));

  for (std::size_t i{1}; i < (c.size()) && (c[i]); ++i)
  {
    LiteralIo.Write(c[i]);
  }
}

/// @brief The implementation is used to parses a string enclosed in quotes,
/// dealing with escape sequences.Parses a string enclosed in quotes,
/// dealing with escape sequences. Assumes the opening quote has already been parsed.
/// @tparam Context Variable is the type of datatype of parameter.
/// @param[in] context Variable is of type reference of Context.
/// @returns It returns instance of StringView.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611160
template <typename Context>
ara::core::StringView ParseString(Context& context)
{
  LiteralIo<Context> LiteralIo{context};

  enum
  {
    kCharacter,
    kEscapeSequence,
    kUtf16Sequence,
    kClosed
  } state{kCharacter};

  std::array<char, 4> utf16Seq{};
  std::size_t utf16SeqOffset{0};
  std::uint16_t highSurrogate{0};

  char c;

  while ((state != kClosed) && ((c = LiteralIo.Read()) != 0))
  {
    switch (state)
    {
    case kCharacter:
      if (c == '\\')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kEscapeSequence;
      }
      else if (highSurrogate != 0)
      {
        throw ParseError(context, ParseError::kExpectedUtf16LowSurrogate);
      }
      else if (c == '"')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kClosed;
      }
      else if ((c >= 0x1) && (c <= 0x1F))
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
      {
        throw ParseError(context, ParseError::kUnescapedControlCharacter);
      }
      else
      {
        LiteralIo.Write(c);
      }
      break;

    case kEscapeSequence:
      state = {kCharacter};

      switch (c)
      {
      case '"':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('"');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case '\\':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('\\');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case '/':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('/');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case 'b':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('\b');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case 'f':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('\f');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case 'n':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('\n');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case 'r':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('\r');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case 't':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        LiteralIo.Write('\t');
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        break;
      case 'u':
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
        state = kUtf16Sequence;
        break;
      default:
        throw ParseError(context, ParseError::kInvalidEscapeSequence);
      }
      break;

    case kUtf16Sequence:
      if (!IsHexDigit(c))
      {
        throw ParseError(context, ParseError::kInvalidEscapeSequence);
      }

      utf16Seq[utf16SeqOffset++] = c;

      if (utf16SeqOffset == utf16Seq.size())
      {
        try
        {
          const std::uint16_t codeUnit{ParseUtf16EscapeSequence(utf16Seq)};

          if ((codeUnit == 0) && (highSurrogate == 0))
          {
            throw ParseError(context, ParseError::kNullUtf16Character);
          }

          if (highSurrogate != 0)
          {
            // We were waiting for the low surrogate
            // (that now is codeUnit)
            WriteUtf8Char(LiteralIo, Utf16ToUtf8(highSurrogate, codeUnit));
            highSurrogate = 0;
          }
          else if ((codeUnit >= 0xD800) && (codeUnit <= 0xDBFF))
          // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
          {
            highSurrogate = codeUnit;
          }
          else
          {
            WriteUtf8Char(LiteralIo, Utf16ToUtf8(codeUnit, 0));
          }
        }
        catch (const EncodingError&)
        {
          throw ParseError(context, ParseError::kInvalidUtf16Character);
        }

        utf16SeqOffset = 0;

        state = kCharacter;
      }
      break;

    // LCOV_EXCL_START
    case kClosed:
      throw std::runtime_error(
          "[minijson_reader] this line should never be reached, "
          "please file a bug report");
      // LCOV_EXCL_STOP
    }
  }

  if (state != kClosed)
  {
    throw ParseError(context, ParseError::kUnterminatedValue);
  }

  return LiteralIo.Finalize();
}

/// @brief Type trait to check if T is a specialization of std::optional.
/// @tparam T Variable is the type of datatype of parameter.
template <typename T>
struct IsStdOptional : std::false_type
{
};

/// @brief Type trait to check if U is a specialization of std::optional.
/// @tparam U Variable is the type of datatype of parameter.
template <typename U>
struct IsStdOptional<ara::core::Optional<U>> : std::true_type
{
};

/// @brief Standard trick to prevent a static_assert() from always going off
/// when it is inside a final "else" of a sequence of "if constexpr" blocks.
template <typename>
inline constexpr bool isTypeDependentFalse{false};

} // namespace detail

/// @brief The enum defines various data types.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
enum ValueType
{
  kString,
  kNumber,
  kBoolean,
  kObject,
  kArray,
  kNull
};

/// @brief Encapsulation of class BadValueCast.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611055
class BadValueCast final : public std::invalid_argument
{
 public:
  /// @brief Namespace of invalid_argument.Bring a specific member from the
  /// namespace into the current scope.
  using std::invalid_argument::invalid_argument;
};
/// @brief Definition of ValueAs.
/// @tparam T Variable is the type of datatype of parameter.
/// @tparam Enable Variable is the type of datatype of parameter.
template <typename T, typename Enable = void>
struct ValueAs;

/// @brief Encapsulation of class value.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611081
class value final
{
 public:
  /// @brief Implementation of default constructor.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  explicit value() = default;

  /// @brief Implementation of Parameterised constructor.
  /// @param[in] type Variable is of type const ValueType.
  /// @param[in] rawValue Variable is of type ara::core::StringView.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611082
  explicit value(const ValueType type, ara::core::StringView const rawValue = "") :
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      mType_(type), mRawValue_(rawValue)
  {
  }

  /// @brief Implementation is used to return value type.
  /// @returns It returns type of type enum ValueType.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611083
  ValueType Type() const
  {
    return mType_;
  }

  /// @brief Implementation is used to return raw value.
  /// @returns It returns the raw value of type ara::core::StringView
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611084
  ara::core::StringView Raw() const
  {
    return mRawValue_;
  }

  /// @brief This is Implementation of As().
  /// @tparam T Variable is the type of datatype of parameter.
  /// @returns It returns template type T.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611085
  template <typename T>
  T As() const
  {
    return ValueAs<T>()(*this);
  }

  /// @brief This is Implementation of To().
  /// @tparam T Variable is the type of datatype of parameter.
  /// @param[in] dest Variable is of type T.
  /// @returns It returns the reference of typename T.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611086
  template <typename T>
  T& To(T& dest) const
  {
    dest = As<T>();
    return dest;
  }

 private:
  /// @brief To store the type.
  ValueType mType_{kNull};

  /// @brief To store the raw value.
  ara::core::StringView mRawValue_{"null"};
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
}; // class value

/// @brief Fallback behavior of value::As<T>() when no user-provided ValueAs
/// specialization for T exists.This function is also meant to be called directly
/// by the user in case they need to fall back to the default behavior inside their
/// ValueAs specialization for T, for whatever reason.
/// @tparam T Variable is the type of datatype of parameter.
/// @param[in] v VAriable is of type value class.
/// @returns It returns type of the function.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611146
template <typename T>
T ValueAsDefault(const value v)
{
  if constexpr (detail::IsStdOptional<T>())
  {
    /// @brief Using a type Conversion
    using U = typename T::ValueType;
    static_assert(!detail::IsStdOptional<U>(),
        "it appears that T is ara::core::Optional<ara::core::Optional<...>> which is "
        "unlikely to be what you meant to do");

    if (v.Type() == kNull)
    {
      return ara::core::nullopt;
    }

    return v.As<U>();
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A5.1.1 Use of magic string literal.
  else // not an optional
  {
    switch (v.Type())
    {
    case kNull:
      throw BadValueCast(
          "cannot Call value::As<T>() on values of type kNull: "
          "consider checking value::Type() first, or use "
          "value::As<ara::core::Optional<T>>()");

    case kObject:
      throw BadValueCast(
          "cannot Call value::As<T>() on values of type kObject: "
          "you have to Call ParseObject() on the same context");

    case kArray:
      throw BadValueCast(
          "cannot Call value::As<T>() on values of type kArray: "
          "you have to Call ParseArray() on the same context");

    case kString:
    case kBoolean:
    case kNumber:
      break;
    }
    /// @brief To store the raw of ara::core::StringView type.
    ara::core::StringView const raw{v.Raw()};
    if constexpr (std::is_same<T, ara::core::StringView>::value)
    {
      if (v.Type() != kString)
      {
        throw BadValueCast("value::As<T>(): value type is not kString");
      }

      return raw;
    }
    else if constexpr (is_same<T, bool>::value)
    {
      if (v.Type() != kBoolean)
      {
        throw BadValueCast("value::As<T>(): value type is not kBoolean");
      }

      // If this value comes from ParseObject() or ParseArray(),
      // as it should, we know that raw is either "true" or "false".
      // However, we do a paranoia check for emptiness.
      return (!raw.empty()) && (raw[0] == 't');
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    }
    else if constexpr (std::is_arithmetic<T>::value)
    {
      if (v.Type() != kNumber)
      {
        throw BadValueCast("value::As<T>(): value type is not kNumber");
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      }

      T result{}; // value initialize to silence compiler warnings
      /*
            const auto begin = raw.data();
            const auto end = raw.data() + raw.size();
            const auto [parse_end, error] = std::from_chars(begin, end, result);
            if (parse_end != end || error != std::errc())
            {
                throw std::range_error(
                    "value::As<T>() could not parse the number");
            }
*/
      iox::cxx::convert::fromString(raw.data(), result);
      return result;
    }
    else // unsupported type
    {
      // We need the predicate of static_assert() to depend on T,
      // otherwise the assert always goes off.
      static_assert(detail::isTypeDependentFalse<T>,
          "value::As<T>(): T is not one of the supported types "
          "(std::string_view, bool, arithmetic types, plus all of the "
          "above wrapped in std::optional)");
    }
    // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A5.1.1 Use of magic string literal.

/// @brief Definition of struct ValueAs.
/// @tparam T Variable is the type of datatype of parameter.
/// @tparam Enable Variable is the type of datatype of parameter.
template <typename T, typename Enable>
struct ValueAs final
{
  /// @brief Implementation is of operator overloading.
  /// @param[in] v Variable of type const value.
  /// @returns It returns a type of parameter.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  T operator()(const value v) const
  {
    return ValueAsDefault<T>(v);
  }
};

namespace detail
{
/// @brief Implementation is convenience function to consume a verbatim sequence of characters
/// in a value not enclosed in quotes (in practice, kNull and kBoolean).
/// @tparam Context Variable is the type of datatype of parameter.
/// @tparam Size Variable of type std::size_t.
/// @param[in] LiteralIo Variable of type LiteralIo<Context>.
/// @param[in] sequence Variable of type array of char.
/// @returns It returns the value termination character (e.g. ',').
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611161
template <typename Context, std::size_t Size>
char Consume(LiteralIo<Context>& LiteralIo, const std::array<char, Size>& sequence)
{
  for (const char expected : sequence)
  {
    const char read{LiteralIo.Read()};
    if (read == 0)
    {
      throw ParseError(LiteralIo.context(), ParseError::kUnterminatedValue);
    }
    if (read != expected)
    {
      throw ParseError(LiteralIo.context(), ParseError::kInvalidValue);
    }
    LiteralIo.Write(read);
  }

  const char read{LiteralIo.Read()};
  if (read == 0)
  {
    throw ParseError(LiteralIo.context(), ParseError::kUnterminatedValue);
  }
  if (!IsValueTermination(read))
  {
    throw ParseError(LiteralIo.context(), ParseError::kInvalidValue);
  }
  return read;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Implementation Parses primitive values that are not enclosed in quotes
/// (i.e. kNull, kBoolean and kNumber).
/// @tparam Context Variable is the type of datatype of parameter.
/// @param[in] context Variable is of type Context.
/// @param[in] firstChar Variable is of type char.
/// @returns It returns the value and its termination character (e.g. ',').
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611162
template <typename Context>
std::tuple<value, char> ParseUnquotedValue(Context& context, const char firstChar)
{
  LiteralIo<Context> LiteralIo{context};

  char c{firstChar};

  // Cover "null", "true" and "false" cases
  switch (c)
  {
  case 'n': // "null"
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      LiteralIo.Write(c);
      c = Consume(LiteralIo, std::array<char, 3>{'u', 'l', 'l'});
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      return {value(kNull, LiteralIo.Finalize()), c};
      break;
    }

  case 't': // "true"
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      LiteralIo.Write(c);
      c = Consume(LiteralIo, std::array<char, 3>{'r', 'u', 'e'});
      return {value(kBoolean, LiteralIo.Finalize()), c};
      break;
    }
  case 'f': // "false"
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      LiteralIo.Write(c);
      c = Consume(LiteralIo, std::array<char, 4>{'a', 'l', 's', 'e'});
      return {value(kBoolean, LiteralIo.Finalize()), c};
      break;
    default:
      break;
    }
  }

  // We are in the kNumber case.
  // Let's check that the number looks OK according to the JSON
  // specification, but let's not convert it yet
  // (that happens in value::As<T>() only as required).

  if (IsValueTermination(c))
  {
    throw ParseError(context, ParseError::kExpectedValue);
  }

  enum
  {
    kSignOrFristDigit,
    kFristDigit,
    kAfterLeadingZero,
    kIntegralPart,
    kFractionalPartFirstDigit,
    kFractionalPart,
    kExponentSingOrFirstDigit,
    kExponentFirstDigit,
    kExponent,
  } state{kSignOrFristDigit};

  while (true)
  {
    if (c == 0)
    {
      throw ParseError(context, ParseError::kUnterminatedValue);
    }
    if (IsValueTermination(c))
    {
      break;
    }

    switch (state)
    {
    case kSignOrFristDigit:
      if (c == '-') // leading plus sign not allowed
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kFristDigit;
        break;
      }
      [[fallthrough]];
    case kFristDigit:
      if (c == '0')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        // If zero is the first digit, then it must be the ONLY digit
        // of the integral part
        state = kAfterLeadingZero;
        break;
      }
      if (IsDigit(c))
      {
        state = kIntegralPart;
        break;
      }
      throw ParseError(context, ParseError::kInvalidValue);

    case kIntegralPart:
      if (IsDigit(c))
      {
        break;
      }
      [[fallthrough]];
    case kAfterLeadingZero:
      if (c == '.')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kFractionalPartFirstDigit;
        break;
      }
      if ((c == 'e') || (c == 'E'))
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kExponentSingOrFirstDigit;
        break;
      }
      throw ParseError(context, ParseError::kInvalidValue);

    case kFractionalPart:
      if ((c == 'e') || (c == 'E'))
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kExponentSingOrFirstDigit;
        break;
      }
      [[fallthrough]];
    case kFractionalPartFirstDigit:
      if (IsDigit(c))
      {
        state = kFractionalPart;
        break;
      }
      throw ParseError(context, ParseError::kInvalidValue);

    case kExponentSingOrFirstDigit:
      if ((c == '+') || (c == '-'))
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kExponentFirstDigit;
        break;
      }
      [[fallthrough]];
    case kExponentFirstDigit:
    case kExponent:
      if (IsDigit(c))
      {
        state = kExponent;
        break;
      }
      throw ParseError(context, ParseError::kInvalidValue);
    }

    LiteralIo.Write(c);
    c = LiteralIo.Read();
  }

  switch (state)
  {
  case kAfterLeadingZero:
  case kIntegralPart:
  case kFractionalPart:
  case kExponent:
    break;
  default:
    throw ParseError(context, ParseError::kInvalidValue);
  }

  return {value(kNumber, LiteralIo.Finalize()), c};
}

/// @brief Implementation is helper function of ParseObject() and ParseArray() dealing with
/// the opening bracket/brace of arrays and objects in presence of nesting.
/// @tparam Context Variable is the type of datatype of parameter.
/// @param[in,out] context Variable of type constant Context.
/// @param[in,out] c Variable of type char.
/// @param[in,out] isMustRead Variable of type bool.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611163
template <typename Context>
void ParseInit(const Context& context, char& c, bool& isMustRead) noexcept
{
  switch (context.NestedStatus())
  {
  case Context::kNestedStatusNone:
    isMustRead = true;
    break;

  case Context::kNestedStatusObject:
    c = '{';
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    // Since we are parsing a nested object, we already read an opening
    // brace. The main loop does not need to read a character from the
    // input.
    isMustRead = false;
    break;

  case Context::kNestedStatusArray:
    // Since we are parsing a nested array, we already read an opening
    // bracket. The main loop does not need to read a character from the
    // input.
    c = '[';
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    isMustRead = false;
    break;
  default:
    break;
  }
}

/// @brief Implementation is helper function of ParseObject() and ParseArray() parsing JSON values.
/// In case the value is a nested kObject or kArray, returns a placeholder value.
/// @tparam Context Variable is the type of datatype of parameter.
/// @param[in,out] context Variable of type Context.
/// @param[in,out] c Variable of type char.
/// @param[in,out] isMustRead Variable of type bool.
/// @returns It returns object of class value.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611164
template <typename Context>
value ParseValue(Context& context, char& c, bool& isMustRead)
{
  value res;
  switch (c)
  {
  case '{':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      context.BeginNested(Context::kNestedStatusObject);
      res = value(kObject);
      break;
    }

  case '[':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      context.BeginNested(Context::kNestedStatusArray);
      res = value(kArray);
      break;
    }

  case '"':
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    {
      res = value(kString, ParseString(context));
      break;
    }

  default: // kBoolean, kNull or kNumber
  {
    value v;
    std::tie(v, c) = ParseUnquotedValue(context, c);
    // c contains the character after the value, no need to read again
    // in the main loop
    isMustRead = false;
    return v;
  }
  }
  return res;
}

} // namespace detail

/// @brief Implementation is used to return parse object offset value.
/// @tparam Context Variable is the type of datatype of parameter.
/// @tparam Handler Variable is the type of datatype of parameter.
/// @param[in,out] context Variable of type Context.
/// @param[in] handler Variable of type Handler.
/// @returns It returns parse object offset value.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895 609487 609489 606813
/// @design 611147
template <typename Context, typename Handler>
std::size_t ParseObject(Context& context, Handler&& handler)
{
  const std::size_t readOffset{context.ReadOffset()};

  const std::size_t nestingLevel{context.NestingLevel()};
  if (nestingLevel > MJR_NESTING_LIMIT)
  {
    throw ParseError(context, ParseError::kExpectedNestingLimit);
  }

  char c{0};
  bool isMustRead{false};

  detail::ParseInit(context, c, isMustRead);
  context.ResetNestedStatus();

  enum
  {
    kOpeningBracket,
    kFieldNameOrClosingBracet, // in case the object is empty
    kFieldName,
    kColon,
    kFieldValue,
    kCommaOrClosingBracket,
    kEnd
  } state{kOpeningBracket};

  ara::core::StringView fieldName{""};

  while (state != kEnd)
  {
    if (context.NestingLevel() != nestingLevel)
    {
      throw ParseError(context, ParseError::kNestedObjectOrArrayNotParsed);
    }

    if (isMustRead)
    {
      c = context.Read();
    }

    isMustRead = true;

    if (detail::IsWhitespace(c))
    {
      continue;
    }

    switch (state)
    {
    case kOpeningBracket:
      if (c != '{')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        throw ParseError(context, ParseError::kExpectedOpeningBracket);
      }
      state = kFieldNameOrClosingBracet;
      break;

    case kFieldNameOrClosingBracet:
      if (c == '}')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kEnd;
        break;
      }
      [[fallthrough]];

    case kFieldName:
      if (c != '"')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        throw ParseError(context, ParseError::kExpectedOpeningQuote);
      }
      fieldName = detail::ParseString(context);
      state = kColon;
      break;

    case kColon:
      if (c != ':')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        throw ParseError(context, ParseError::kExpectedColon);
      }
      state = kFieldValue;
      break;

    case kFieldValue:
    {
      const value v{detail::ParseValue(context, c, isMustRead)};

      // Try calling the Handler with the context as the last argument
      if constexpr (eggs::is_invocable_v<decltype(handler), decltype(fieldName), decltype(v),
                        decltype(context)>)
      {
        eggs::Invoke(handler, fieldName, v, context);
      }
      else
      {
        // Now try again without the context. Generate a compile
        // error if it does not work.
        eggs::Invoke(handler, fieldName, v);
      }
    }
      state = kCommaOrClosingBracket;
      break;

    case kCommaOrClosingBracket:
      if (c == ',')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kFieldName;
      }
      else if (c == '}')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kEnd;
      }
      else
      {
        throw ParseError(context, ParseError::kExpectedCommaOrClosingBracket);
      }
      break;

    // LCOV_EXCL_START
    case kEnd:
      throw std::runtime_error(
          "[minijson_reader] this line should never be reached, "
          "please file a bug report");
      // LCOV_EXCL_STOP

    default:
      break;
    }

    if (c == 0)
    {
      throw std::runtime_error( // LCOV_EXCL_LINE
          "[minijson_reader] this line should never be reached, "
          "please file a bug report");
    }
  }

  context.EndNested();

  return context.ReadOffset() - readOffset;
}

/// @brief Implementation is used to return parse array offset value.
/// @tparam Context Variable is the type of datatype of parameter.
/// @tparam Handler Variable is the type of datatype of parameter.
/// @param[in,out] context Variable of type Context.
/// @param[in,out] handler Variable of type Handler.
/// @returns It returns parse array offset value.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895 609487 609488 606812
/// @design 611148
template <typename Context, typename Handler>
std::size_t ParseArray(Context& context, Handler&& handler)
{
  const std::size_t readOffset{context.ReadOffset()};

  const std::size_t nestingLevel{context.NestingLevel()};
  if (nestingLevel > MJR_NESTING_LIMIT)
  {
    throw ParseError(context, ParseError::kExpectedNestingLimit);
  }

  char c{0};
  bool isMustRead{false};

  detail::ParseInit(context, c, isMustRead);
  context.ResetNestedStatus();

  enum
  {
    kOpeningBracket,
    kValueOrClosingBracket, // in case the array is empty
    kValue,
    kCommaOrClosingBracket,
    kEnd
  } state{kOpeningBracket};

  while (state != kEnd)
  {
    if (context.NestingLevel() != nestingLevel)
    {
      throw ParseError(context, ParseError::kNestedObjectOrArrayNotParsed);
    }

    if (isMustRead)
    {
      c = context.Read();
    }

    isMustRead = true;

    if (detail::IsWhitespace(c))
    {
      continue;
    }

    switch (state)
    {
    case kOpeningBracket:
      if (c != '[')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        throw ParseError(context, ParseError::kExpectedOpeningBracket);
      }
      state = kValueOrClosingBracket;
      break;

    case kValueOrClosingBracket:
      if (c == ']')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kEnd;
        break;
      }
      [[fallthrough]];

    case kValue:
    {
      const value v{detail::ParseValue(context, c, isMustRead)};

      // Try calling the Handler with the context as the last argument
      if constexpr (eggs::is_invocable_v<decltype(handler), decltype(v), decltype(context)>)
      {
        eggs::Invoke(handler, v, context);
      }
      else
      {
        // Now try again without the context. Generate a compile
        // error if it does not work.
        eggs::Invoke(handler, v);
      }
    }
      state = kCommaOrClosingBracket;
      break;

    case kCommaOrClosingBracket:
      if (c == ',')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kValue;
      }
      else if (c == ']')
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      {
        state = kEnd;
      }
      else
      {
        throw ParseError(context, ParseError::kExpectedCommaOrClosingBracket);
      }
      break;

    // LCOV_EXCL_START
    case kEnd:
      throw std::runtime_error(
          "[minijson_reader] this line should never be reached, "
          "please file a bug report");
      // LCOV_EXCL_STOP
    default:
      break;
    }

    if (c == 0)
    {
      throw std::runtime_error( // LCOV_EXCL_LINE
          "[minijson_reader] this line should never be reached, "
          "please file a bug report");
    }
  }

  context.EndNested();

  return context.ReadOffset() - readOffset;
}

namespace detail
{
/// @brief Encapsulation of class Ignore.
/// @tparam Context Variable is the type of datatype of parameter.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611093
template <typename Context>
class Ignore final
{
 public:
  /// @brief Implementation is parameterised constructor.
  /// @param[in,out] context Variable of type Context.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611094
  explicit Ignore(Context& context) noexcept : mContext_(context)
  {
  }

  /// @brief Copy constructor shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  Ignore(const Ignore&) = delete;

  /// @brief Move constructor shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  Ignore(Ignore&&) = delete;

  /// @brief Copy assignment operator shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  Ignore& operator=(const Ignore&) = delete;

  /// @brief Move assignment operator shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  Ignore& operator=(Ignore&&) = delete;

  /// @brief This is function Call operator of the class Ignore.
  /// @param[in] StringView An object convertible to string_view containing characters to locate.
  /// @param[in] value Instance of value class.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611095
  void operator()(ara::core::StringView, value) const
  {
    (*this)();
  }

  /// @brief This is function Call operator of the class Ignore.
  /// @param[in] value Instance of value class.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611096
  void operator()(value) const
  {
    (*this)();
  }

  /// @brief This is function Call operator of the class Ignore.It will check for the nested status.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611097
  void operator()() const
  {
    switch (mContext_.NestedStatus())
    {
    case Context::kNestedStatusNone:
      break;
    case Context::kNestedStatusObject:
      ParseObject(mContext_, *this);
      break;
    case Context::kNestedStatusArray:
      ParseArray(mContext_, *this);
      break;
    }
  }

 private:
  /// @brief To store the context.
  Context& mContext_;
}; // class Ignore

/// @brief Encapsulation of dispatcher_error. base Base for UnhandledFieldError and MissingFieldError.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611141
class DispatcherErrorBase : public std::exception
{
 public:
  /// @brief Implementation is parameterised constructor.copies a number of elements to a new location.
  /// Copies exactly count values from the range beginning at first to the range beginning at result.
  /// @param[in] fieldName Variable of type ara::core::StringView.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611142
  explicit DispatcherErrorBase(ara::core::StringView const fieldName) noexcept :
      mFieldNameTruncatedLength_(std::min(stkMaxFieldNameLength, fieldName.size()))
  {
    std::copy_n(fieldName.begin(), mFieldNameTruncatedLength_, mFieldNameTruncated_.begin());
  }

  /// @brief Implementation is used to return data of field and length of field.
  /// @returns It returns data and length of field.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611143
  ara::core::StringView FieldNameTruncated() const noexcept
  {
    return {mFieldNameTruncated_.data(), mFieldNameTruncatedLength_};
  }

  /// @brief Variable to store maximum field length.
  inline static constexpr std::size_t stkMaxFieldNameLength{55};

 private:
  /// @brief The temptation of storing a the field name here as a std::string_view
  /// is almost irresistible, but it would tie the validity of this exception
  /// to that of the dispatcher or, even worse, the parsing context
  std::size_t mFieldNameTruncatedLength_{0};

  ///@brief To store the array of the char.
  std::array<char, stkMaxFieldNameLength + 1> mFieldNameTruncated_{};
}; // class DispatcherErrorBase

} // namespace detail

/// @brief Implementation used to Ignore.
/// @tparam Context Variable is the type of datatype of parameter.
/// @param[in] context Variable is of type  Context.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611149
template <typename Context>
void Ignore(Context& context)
{
  detail::Ignore<Context> Ignore(context);
  Ignore();
}

namespace handlers
{
namespace detail
{
/// @brief Definition of the HandlerTag. Tags to declare Handler Traits.
struct HandlerTag
{
};

/// @brief Definition of the struct FieldSpecificHandlerTag.
struct FieldSpecificHandlerTag
{
};

/// @brief Definition of the struct required_field_handle_rtag.
struct RequiredFieldHandlerTag
{
};

/// @brief Definition of the struct IgnoreHandlerTag.
struct IgnoreHandlerTag
{
};

/// @brief Encapsulation of FieldSpecificHandlerBase. Base class for field-specific handlers.
/// @tparam Functor Variable is the type of datatype of parameter.
/// @tparam Tag Variadic function template that takes variable number of arguments.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611121
template <typename Functor, typename... Tag>
class FieldSpecificHandlerBase
    : public HandlerTag
    , public FieldSpecificHandlerTag
    , public Tag...
{
 public:
  /// @brief Implementation of parameterised constructor.
  /// @param[in] fieldName Variable of type ara::core::StringView.
  /// @param[in] functor Variable of type Functor.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611122
  explicit FieldSpecificHandlerBase(ara::core::StringView const fieldName, Functor functor) :
      mFieldName_(fieldName), mFunctor_(std::move(functor))
  {
  }

  /// @brief Implementation is used to get field name.
  /// @returns Method returns field name of type ara::core::StringView.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611123
  ara::core::StringView FieldName() const noexcept
  {
    return mFieldName_;
  }

  /// @brief Implementation is used to get functor.
  /// @returns Method returns the instance of Functor
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611124
  const Functor& functor() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mFunctor_;
  }

  /// @brief The implementation of function Call operator.
  /// @tparam Context Variable is the type of datatype of parameter.
  /// @tparam Target Variadic function template that takes variable number of arguments.
  /// @param[in] parsedFieldName Variable is of type ara::core::StringView.
  /// @param[in] value Variable is of type value.
  /// @param[in] context Variable is of type Context.
  /// @param[in] target Variadic function that takes variable number of arguments.
  /// @returns It returns false parsed field name is not equal to field name else it will return true.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611125
  template <typename Context, typename... Target>
  bool operator()(ara::core::StringView const parsedFieldName, const value value, Context& context,
      Target&... target) const
  {
    if (parsedFieldName != mFieldName_)
    {
      return false;
    }

    // Try calling the functor with the context as the last argument
    if constexpr (eggs::is_invocable_v<decltype(mFunctor_), decltype(target)..., decltype(value),
                      decltype(context)>)
    {
      eggs::Invoke(mFunctor_, target..., value, context);
    }
    else
    {
      // Now try again without the context. Generate a compile error if
      // it does not work.
      eggs::Invoke(mFunctor_, target..., value);
    }

    return true;
  }

 private:
  /// @brief To store the field name.
  ara::core::StringView mFieldName_;

  /// @brief To store the functor.
  Functor mFunctor_;
}; // class FieldSpecificHandlerBase

/// @brief Encapsulation of AnyHandlerBase. Base class for non-field-specific handlers.
/// @tparam Functor Variable is the type of datatype of parameter.
/// @tparam Tag Variable is the type of datatype of parameter.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611075
template <typename Functor, typename... Tag>
class AnyHandlerBase
    : public HandlerTag
    , public Tag...
{
 public:
  /// @brief Implementation of parameterised constructor.
  /// @param[in] functor Variable is of type Functor.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611076
  AnyHandlerBase(Functor functor) : mFunctor_(std::move(functor))
  {
  }

  /// @brief Implementation is used to get functor.
  /// @returns Method returns the instance of Functor
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611077
  const Functor& functor() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mFunctor_;
  }

  /// @brief The implementation of function Call operator.
  /// @tparam Context Variable is the type of datatype of parameter.
  /// @tparam Target Variadic function template that takes variable number of arguments.
  /// @param[in] parsedFieldName Variable is of type ara::core::StringView.
  /// @param[in] value Variable is of type value.
  /// @param[in] context Variable is of type Context.
  /// @param[in] target Variadic function that takes variable number of arguments.
  /// @returns It returns false parsed field name is not equal to field name else it will return true.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611078
  template <typename Context, typename... Target>
  bool operator()(ara::core::StringView const parsedFieldName, const value value, Context& context,
      Target&... target) const
  {
    // Try calling the functor with the context as the last argument
    if constexpr (eggs::is_invocable_v<decltype(mFunctor_), decltype(target)..., decltype(parsedFieldName),
                      decltype(value), decltype(context)>)
    {
      return eggs::Invoke(mFunctor_, target..., parsedFieldName, value, context);
    }
    else
    {
      // Now try again without the context. Generate a compile error if
      // it does not work.
      return eggs::Invoke(mFunctor_, target..., parsedFieldName, value);
    }
  }

 private:
  /// @brief To store the functor.
  Functor mFunctor_;
}; // class AnyHandlerBase

/// @brief Definition of IgnoreFunctor. Functor for all handlers which Ignore the field
struct IgnoreFunctor final
{
  /// @brief The implementation of function Call operator.
  /// @tparam Args Variadic function Template that takes variable number of arguments.
  /// @param[in] args Variadic function that takes variable number of arguments.
  /// @returns It return true if able to store the value in args_tuple.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename... Args>
  bool operator()(Args&&... args) const
  {
    // The context is the last argument we are being passed, extract it
    const auto args_tuple{std::forward_as_tuple(std::forward<Args>(args)...)};
    auto& context{std::get<sizeof...(args) - 1>(args_tuple)};

    // Call Ignore to discard the current value
    Ignore(context);

    return true;
  }
}; // struct IgnoreFunctor

} // namespace detail

/// @brief The encapsulation of Handler.
/// @tparam Functor Variable is the type of datatype of parameter.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611079
template <typename Functor>
class Handler final : public detail::FieldSpecificHandlerBase<Functor, detail::RequiredFieldHandlerTag>
{
 public:
  /// @brief The implementation of parameterised constructor.
  /// @param[in] fieldName Variable is of type ara::core::StringView.
  /// @param[in] functor Variable is of type Functor.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611080
  Handler(ara::core::StringView const fieldName, Functor functor) :
      detail::FieldSpecificHandlerBase<Functor, detail::RequiredFieldHandlerTag>(
          fieldName, std::move(functor))
  {
  }
}; // class Handler

/// @brief The encapsulation of OptionalHandler.
/// @tparam Functor Variable is the type of datatype of parameter.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611139
template <typename Functor>
class OptionalHandler final : public detail::FieldSpecificHandlerBase<Functor>
{
 public:
  /// @brief The implementation of parameterised constructor.
  /// @param[in] fieldName Variable is of type ara::core::StringView.
  /// @param[in] functor Variable is of type Functor.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611140
  OptionalHandler(ara::core::StringView const fieldName, Functor functor) :
      detail::FieldSpecificHandlerBase<Functor>(fieldName, std::move(functor))
  {
  }
}; // class OptionalHandler

/// @brief The Encapsulation of AnyHandler.
/// @tparam Functor Variable is the type of datatype of parameter.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611067
template <typename Functor>
class AnyHandler final : public detail::AnyHandlerBase<Functor>
{
 public:
  /// @brief The implementation of parameterised constructor.
  /// @param[in] functor Variable is of type Functor.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611068
  AnyHandler(Functor functor) : detail::AnyHandlerBase<Functor>(std::move(functor))
  {
  }
}; // class AnyHandler

/// @brief Encapsulation of class AnyHandler.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611056
class IgnoreHandler final
    : public detail::FieldSpecificHandlerBase<detail::IgnoreFunctor, detail::IgnoreHandlerTag>
{
 public:
  /// @brief The implementation of parameterised constructor.
  /// @param[in] fieldName Variable is of type ara::core::StringView.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611057
  IgnoreHandler(ara::core::StringView const fieldName) :
      detail::FieldSpecificHandlerBase<detail::IgnoreFunctor, detail::IgnoreHandlerTag>(
          fieldName, detail::IgnoreFunctor())
  {
  }
}; // class IgnoreHandler

/// @brief Encapsulation of class IgnoreAnyHandler.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611058
class IgnoreAnyHandler final : public detail::AnyHandlerBase<detail::IgnoreFunctor, detail::IgnoreHandlerTag>
{
 public:
  /// @brief Implementation of default constructor.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611059
  IgnoreAnyHandler() :
      detail::AnyHandlerBase<detail::IgnoreFunctor, detail::IgnoreHandlerTag>(detail::IgnoreFunctor())
  {
  }
}; // class IgnoreAnyHandler

/// @brief Definition of the Traits.
/// @tparam Handler Variable is the type of datatype of parameter.
/// @tparam Enable Variable is the type of datatype of parameter and initialised as void.
template <typename Handler, typename Enable = void>
struct Traits;

/// @brief Definition of the Traits.
/// @tparam Handler Variable is the type of datatype of parameter.
template <typename Handler>
struct Traits<Handler,
    std::enable_if_t<std::is_base_of<detail::HandlerTag, std::remove_reference_t<Handler>>::value>>
    final
{
  /// @brief Definition of the isFieldSpecific.
  static inline constexpr bool isFieldSpecific{
      std::is_base_of<detail::FieldSpecificHandlerTag, std::remove_reference_t<Handler>>::value};
  /// @brief Definition of the isRequiredField.
  static inline constexpr bool isRequiredField{
      std::is_base_of<detail::RequiredFieldHandlerTag, std::remove_reference_t<Handler>>::value};
  /// @brief Definition of the isIgnore.
  static inline constexpr bool isIgnore{
      std::is_base_of<detail::IgnoreHandlerTag, std::remove_reference_t<Handler>>::value};
};

} // namespace handlers

/// @brief Encapsulation of unhandled field error.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611126
class UnhandledFieldError final : public detail::DispatcherErrorBase
{
 public:
  /// @brief Namespace of dispatcher error base.
  using detail::DispatcherErrorBase::DispatcherErrorBase;

  /// @brief The Implementation is used to return a null terminated
  /// character sequence that is used to identify the exception.
  /// @returns It returns a null terminated character sequence that is used to identify the exception.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611127
  virtual const char* what() const noexcept
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    return "a JSON field was not handled";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
}; // class UnhandledFieldError

/// @brief Encapsulation of missing field error.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611065
class MissingFieldError final : public detail::DispatcherErrorBase
{
 public:
  /// @brief Namespace of dispatcher error base.
  using detail::DispatcherErrorBase::DispatcherErrorBase;

  /// @brief The Implementation is used to return a null terminated character sequence that
  /// is used to identify the exception.
  /// @returns It returns a null terminated character sequence that is used to identify the exception.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611066
  virtual const char* what() const noexcept
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    return "at least one required JSON field is missing";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
}; // class MissingFieldError

/// @brief Encapsulation of dispatcher run.
/// @tparam Dispatcher Variable is the type of datatype of parameter.
/// @tparam Target Variadic function Template that takes variable number of arguments.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611132
template <typename Dispatcher, typename... Target>
class DispatcherRun final
{
 public:
  /// @brief The implementation is of parameterised constructor.
  /// @param[in,out] dispatcher Variable is a template argument.
  /// @param[in,out] target Variadic function Template that takes variable number of arguments.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611133
  DispatcherRun(const Dispatcher& dispatcher, Target&... target) noexcept :
      stkMDispatcher_(dispatcher), mTargets_(target...)
  {
  }
  /// @brief Copy constructor shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  DispatcherRun(const DispatcherRun&) = delete;

  /// @brief Move constructor of class DispatcherRun.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  DispatcherRun(DispatcherRun&&) noexcept = default;

  /// @brief Copy assignment operator shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  DispatcherRun& operator=(const DispatcherRun&) = delete;

  /// @brief Move assignment operator shall be disabled.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  DispatcherRun& operator=(DispatcherRun&&) = delete;

  /// @brief Implementation of function Call operator.It will invoke function Call helper.
  /// @tparam Context Variable is the type of datatype of parameter.
  /// @param[in] parsedFieldName Variable is of type ara::core::StringView.
  /// @param[in] value Variable is of type value.
  /// @param[in] context Variable is of type Context.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611134
  template <typename Context>
  void operator()(ara::core::StringView const parsedFieldName, const value value, Context& context)
  {
    // AFAIK, in pre-C++20 we are forced to resort to a helper function
    // rather than use a template lambda which would be more readable
    CallHelper(parsedFieldName, value, context, std::make_index_sequence<Dispatcher::nhandlers>());
  }

  /// @brief Implementation of inspect function.It will invoke inspect_helper.
  /// @tparam Inspector Variable is the type of datatype of parameter.
  /// @param[in] inspector Variable is of type Inspector.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611135
  template <typename Inspector>
  void Inspect(Inspector&& inspector) const
  {
    // AFAIK, in pre-C++20 we are forced to resort to a helper function
    // rather than use a template lambda which would be more readable
    InspectHelper(inspector, std::make_index_sequence<Dispatcher::nhandlers>());
  }

  /// @brief Implementation of EnforceRequired() function.It will invoke inspect.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611136
  void EnforceRequired() const
  {
    Inspect([](const auto& Handler, const std::size_t handleCount) {
      if constexpr (handlers::Traits<decltype(Handler)>::isRequiredField)
      {
        if (handleCount == 0)
        {
          throw MissingFieldError(Handler.FieldName());
        }
      }
    });
  }

 private:
  /// @brief Helper of operator() to get a sequence of indices.
  /// @tparam Context Variable is the type of datatype of parameter.
  /// @tparam I Variadic function Template that takes variable number of arguments.
  /// @param[in] parsedFieldName Variable is of type ara::core::StringView.
  /// @param[in] value Variable is of type value.
  /// @param[in] context Variable is of type Context.
  /// @param[in] I Variadic function that takes variable number of arguments.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611134
  template <typename Context, std::size_t... I>
  void CallHelper(ara::core::StringView const parsedFieldName, const value value, Context& context,
      std::index_sequence<I...>)
  {
    if (!(... || OfferToHandler<I>(parsedFieldName, value, context)))
    {
      throw UnhandledFieldError(parsedFieldName);
    }
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Implementation of inspect_helper method.
  /// @tparam Inspector Variable is the type of datatype of parameter.
  /// @tparam I Variadic function Template that takes variable number of arguments.
  /// @param[in] inspector Variable is of type Inspector.
  /// @param[in] I Variadic function that takes variable number of arguments.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611135
  template <typename Inspector, std::size_t... I>
  void InspectHelper(Inspector& inspector, std::index_sequence<I...>) const
  {
    (..., eggs::Invoke(inspector, std::get<I>(stkMDispatcher_.Handlers()), std::get<I>(mHandleCount_)));
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Offer this field to the I-th Handler of the Dispatcher.
  /// If the Handler chooses to handle it, then increment the I-th
  /// mHandleCount_ entry and return true. Otherwise, return false.
  /// @tparam I Variable is a template argument of type std::size_t.
  /// @tparam Context Variable is the type of datatype of parameter.
  /// @param[in] parsedFieldName Variable is of type ara::core::StringView.
  /// @param[in] value Variable is of type value.
  /// @param[in] context Variable is of type Context.
  /// @returns If the Handler chooses to handle it, then increment the I-th mHandleCount_ entry
  /// and return true. Otherwise, return false.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  /// @design 611134
  template <std::size_t I, typename Context>
  bool OfferToHandler(ara::core::StringView const parsedFieldName, const value value, Context& context)
  {
    std::size_t& handleCount{std::get<I>(mHandleCount_)};
    auto& Handler = std::get<I>(stkMDispatcher_.Handlers());

    return app::Apply(
        [&](Target&... target) {
          if (Handler(parsedFieldName, value, context, target...))
          {
            ++handleCount;
            return true;
          }
          return false;
        },
        mTargets_);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief To store the Dispatcher.
  const Dispatcher& stkMDispatcher_;

  /// @brief To store the variable number of targets.
  std::tuple<Target&...> mTargets_;

  /// @brief Tracks how many times the i-th Handler decided to handle a field
  std::array<std::size_t, Dispatcher::nhandlers> mHandleCount_{};
}; // class DispatcherRun

/// @brief Encapsulation of the Dispatcher.
/// @tparam Handler Variadic function Template that takes variable number of arguments.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611128
template <typename... Handler>
class Dispatcher final
{
 public:
  /// @brief Implementation of parameterised constructor.
  /// @param[in] handler Variadic function that takes variable number of arguments.
  /// @archtrace 608895
  /// @design 611129
  Dispatcher(Handler... handler) : mHandlers_{std::move(handler)...}
  {
  }

  /// @brief The Implementation is of run method.
  /// @tparam Context Variable is the type of datatype of parameter.
  /// @tparam Target Variadic function Template that takes variable number of arguments.
  /// @param[in] context Variable is of type Context.
  /// @param[in] target Variadic function that takes variable number of arguments.
  /// @archtrace 608895
  /// @design 611130
  template <typename Context, typename... Target>
  void Run(Context& context, Target&... target) const
  {
    //   std::cout<<"Run Method Called:: "<<this<<std::endl;
    DispatcherRun<Dispatcher<Handler...>, Target...> Run{*this, target...};
    ara::core::configreader::ParseObject(context, Run);
    Run.EnforceRequired();
  }

  /// @brief The Implementation is used to return handlers.
  /// @returns It returns instance of variable number of handlers.
  /// @archtrace 608895
  /// @design 611131
  const std::tuple<Handler...>& Handlers() const noexcept
  {
    return mHandlers_;
  }

  /// @brief Variable of std::size_t type to store handlers.
  inline static constexpr std::size_t nhandlers{sizeof...(Handler)};

 private:
  /// @brief To store the handlers.
  std::tuple<Handler...> mHandlers_;
}; // class Dispatcher
} // namespace configreader
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_UTILS_CONFIG_READER_INC_PUBLIC_ARA_CORE_CONFIGREADER_CONFIG_READER_H_

#undef MJR_STRINGIFY
#undef MJR_STRINGIFY_HELPER

// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
// - CodingStyle-Naming.Method
//   Guideline:           AUTOSAR CPP14 Guidelines, CodingStyle-Naming.Method required
//   Description:         Methods shall use pascal case.
//   Justification:       This will create another violation for initialization with pascal case. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - CodingStyle-Naming.TemplateConstParameter
//   Guideline:           AUTOSAR CPP14 Guidelines, CodingStyle-Naming.TemplateConstParameter required
//   Description:         Names of template non-type parameters shall be camel case.
//   Justification:       This will create another violation for initialization with camel case. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - CodingStyle-Naming.Function
//   Guideline:           AUTOSAR CPP14 Guidelines, CodingStyle-Naming.Function required
//   Description:         Function names shall be pascal case.
//   Justification:       This will create another violation for initialization with pascal case. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - A5.1.1-1
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic literal.
//   Justification:       This will create another violation for initialization with magic literal. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - A5.1.1-2
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic string literal.
//   Justification:       This will create another violation for initialization with magic string literal.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - A27.0.4
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A27.0.4 required
//   Description:         C-style strings should not be used.
//   Justification:       This will create another violation for initialization with C-style string.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
