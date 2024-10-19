// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2023 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : application.cpp
//
// Purpose     : Integration Testing implementation of core.
// -----------------------------------------------------------------------------------------------------------
#include <ara/core/vector.h>
#include <gtest/gtest.h>
#include <ara/core/steady_clock.h>
#include <ara/log/logging.h>

#include <chrono>
#include <csignal>

#include <atomic>
#include <thread>
#include <unistd.h>
#include <vector>
#include <sm_config.h>
#include <application.h>

namespace it
{
namespace core
{
namespace application
{
/// @brief creates definition of a CoreApp constructor.
CoreApp::CoreApp() : timerObj_(std::make_unique<ara::core::timer::TimerThread>())
{
}
/// @brief creates definition of a ~CoreApp destructor.
CoreApp::~CoreApp()
{
}
/// @brief creates definition of a Init method.
void CoreApp::Init()
{
  for (std::uint8_t i = 0; i < 1; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    IT_Core_0001();
    IT_Core_0002();
    IT_Core_0003();
    IT_Core_0004();
    IT_Core_0005();
    IT_Core_0006();
    IT_Core_0007();
    IT_Core_0008();
    IT_Core_0009();
    IT_Core_0010();
    IT_Core_Pmr_0001();
    IT_Core_Pmr_0003();
    IT_Core_Pmr_0004();
    IT_Core_Pmr_0002();
  }
}
/// @brief creates definition of a handler_fun method.
bool CoreApp::handler_fun(bool flag)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(4000));
  return (!flag);
}
/// @brief creates definition of a ParseArrayInvalidHelper method.
template <std::size_t Length>
void CoreApp::ParseArrayInvalidHelper(const char (&buffer)[Length],
    const ara::core::configreader::ParseError::ErrorReason expectedReason, const char* const expectedWhat)
{
  ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

  try
  {
    ara::core::configreader::ParseArray(ConstBufferContext,
        ara::core::configreader::detail::Ignore<ara::core::configreader::ConstBufferContext>(
            ConstBufferContext));
  }
  catch (const ara::core::configreader::ParseError& e)
  {
    araLog_.LogVerbose() << "ParseArray () should return " << expectedWhat << " error.";
  }
}

/// @brief creates definition of all IT_Core methods.
void CoreApp::IT_Core_0001()
{
  araLog_.LogVerbose() << "###### Timer_TestCases Started ######";

  araLog_.LogVerbose() << " Invoke GetTimerThreadInstance()";
  timerObj_->GetTimerThreadInstance();
  araLog_.LogVerbose() << "GetTimerThreadInstance() should return singleton object.";

  araLog_.LogVerbose() << " Invoke StartPeriodicTimer() ";
  ara::core::timer::MilliSeconds period{5000};
  ara::core::timer::HandlerType PeriodicHandler
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::timer::TimerId id1 = timerObj_->StartPeriodicTimer(period, PeriodicHandler);
  araLog_.LogVerbose() << "StartPeriodicTimer() should return timer id = " << id1;

  araLog_.LogVerbose() << " Invoke StartSingleShotTimer() ";
  ara::core::timer::MilliSeconds timeout{5000};
  ara::core::timer::HandlerType singleShotHandler
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::Result<ara::core::timer::TimerId> id2
      = timerObj_->StartSingleShotTimer(timeout, singleShotHandler);
  araLog_.LogVerbose() << "StartSingleShotTimer() should return timer id = " << id2.Value();

  araLog_.LogVerbose() << " Invoke GetRemainingTime() ";
  ara::core::timer::TimerId id = id1;
  ara::core::Result<ara::core::timer::MilliSeconds> millsec = timerObj_->GetRemainingTime(id);
  araLog_.LogVerbose() << "GetRemainingTime() should return " << millsec.Value() << " MilliSeconds.";

  araLog_.LogVerbose() << " Invoke StopTimer() ";
  ara::core::timer::TimerId timerid = id1;
  ara::core::Result<bool> st = timerObj_->StopTimer(timerid);
  araLog_.LogVerbose() << "StopTimer() should return true = " << st.Value();

  araLog_.LogVerbose() << " Invoke StopTimer() ";
  ara::core::timer::TimerId timerid1 = id2.Value();
  ara::core::Result<bool> st1 = timerObj_->StopTimer(timerid1);
  araLog_.LogVerbose() << "StopTimer() should return true = " << st1.Value();
}

void CoreApp::IT_Core_0002()
{
  araLog_.LogVerbose() << "###### Thread_TestCases Started ######";
  araLog_.LogVerbose() << " Invoke create()";
  ara::core::thread::ThreadName nameOfThread = "Avin";
  ara::core::thread::ThreadBuilder()
      .name(nameOfThread)
      .create(threadObj_, [] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); })
      .expect("Couldn't create a thread.");
  araLog_.LogVerbose() << "create() should return success ";

  araLog_.LogVerbose() << " Invoke GetName()";
  ara::core::thread::ThreadName threadname = threadObj_->GetName();
  araLog_.LogVerbose() << "GetName() should return thread name : " << threadname;
}

void CoreApp::IT_Core_0003()
{
  araLog_.LogVerbose() << "###### Config_Reader_TestCases Started ######";
  ara::sm::config::SmConfig sm_config{};
  sm_config.LoadConfig(std::getenv("SM_CONFIGURATION"));
}

void CoreApp::IT_Core_0004()
{
  araLog_.LogVerbose() << "###### Condition-Variable_TestCases Started ######";
  std::thread waiter([&] {
    std::mutex m;
    iox::units::Duration m_timeToWait = 15_s;
    std::unique_lock<std::mutex> lock(m);
    araLog_.LogVerbose() << " Invoke TimedWait()";
    ara::core::ConditionListener::NotificationVectorT activeNotifications
        = condListener_.TimedWait(lock, m_timeToWait);
    if (activeNotifications.size())
    {
      araLog_.LogVerbose() << "TimedWait () should return Notifications : " << activeNotifications.data();
    }
  });
  sleep(1);
  araLog_.LogVerbose() << "Invoke notify_all()";
  condSignaler_.notify_all();
  waiter.join();
}

void CoreApp::IT_Core_0005()
{
  araLog_.LogVerbose() << "###### ParseArray_Negative-Test-Cases Started ######";

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ("
                          ")";
  ParseArrayInvalidHelper(
      "", ara::core::configreader::ParseError::kExpectedOpeningBracket, "kExpectedOpeningBracket");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([8.2e+62738)";
  ParseArrayInvalidHelper(
      "[8.2e+62738", ara::core::configreader::ParseError::kUnterminatedValue, "kUnterminatedValue");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([,])";
  ParseArrayInvalidHelper("[,]", ara::core::configreader::ParseError::kExpectedValue, "kExpectedValue");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([e+62738])";
  ParseArrayInvalidHelper("[e+62738]", ara::core::configreader::ParseError::kInvalidValue, "kInvalidValue");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([\"\\ufffx\"])";
  ParseArrayInvalidHelper(
      "[\"\\ufffx\"]", ara::core::configreader::ParseError::kInvalidEscapeSequence, "kInvalidEscapeSequence");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([\"foo\nfoo\"])";
  ParseArrayInvalidHelper("[\"foo\nfoo\"]", ara::core::configreader::ParseError::kUnescapedControlCharacter,
      "kUnescapedControlCharacter");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([\"\\ud800\"])";
  ParseArrayInvalidHelper("[\"\\ud800\"]", ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate,
      "kExpectedUtf16LowSurrogate");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([\"\\ud800\\uee00\"])";
  ParseArrayInvalidHelper("[\"\\ud800\\uee00\"]", ara::core::configreader::ParseError::kInvalidUtf16Character,
      "kInvalidUtf16Character");

  araLog_.LogVerbose() << " Invoke ParseArray() with Context parameter value is ([\"\\ud800\\u0000\"])";
  ParseArrayInvalidHelper(
      "[\"\\u0000\"]", ara::core::configreader::ParseError::kNullUtf16Character, "kNullUtf16Character");

  araLog_.LogVerbose() << " ([{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":"
                          "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{"
                          "}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}])";
  ParseArrayInvalidHelper(
      "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":"
      "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{"
      "}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]",
      ara::core::configreader::ParseError::kExpectedNestingLimit, "kExpectedNestingLimit");
}

void CoreApp::IT_Core_0006()
{
  araLog_.LogVerbose() << "###### ParseObject_Negative-Test-Cases Started ######";

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ("
                          ")";
  ParseObjectInvalidHelper(
      "", ara::core::configreader::ParseError::kExpectedOpeningBracket, "kExpectedOpeningBracket");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"x\":8.2e+62738)";
  ParseObjectInvalidHelper(
      "{\"x\":8.2e+62738", ara::core::configreader::ParseError::kUnterminatedValue, "kUnterminatedValue");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"x\": })";
  ParseObjectInvalidHelper(
      "{\"x\": }", ara::core::configreader::ParseError::kExpectedValue, "kExpectedValue");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({:\"foo\"})";
  ParseObjectInvalidHelper(
      "{:\"foo\"}", ara::core::configreader::ParseError::kExpectedOpeningQuote, "kExpectedOpeningQuote");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"x\":8.})";
  ParseObjectInvalidHelper("{\"x\":8.}", ara::core::configreader::ParseError::kInvalidValue, "kInvalidValue");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"\\ufffx\":null})";
  ParseObjectInvalidHelper("{\"\\ufffx\":null}", ara::core::configreader::ParseError::kInvalidEscapeSequence,
      "kInvalidEscapeSequence");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"foo\nfoo\":\"\"})";
  ParseObjectInvalidHelper("{\"foo\nfoo\":\"\"}",
      ara::core::configreader::ParseError::kUnescapedControlCharacter, "kUnescapedControlCharacter");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"\\ud800\":null})";
  ParseObjectInvalidHelper("{\"\\ud800\":null}",
      ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate, "kExpectedUtf16LowSurrogate");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"\\udc00\":null})";
  ParseObjectInvalidHelper("{\"\\udc00\":null}", ara::core::configreader::ParseError::kInvalidUtf16Character,
      "kInvalidUtf16Character");

  araLog_.LogVerbose() << " Invoke ParseObject() with Context parameter value is ({\"\\u0000\":null})";
  ParseObjectInvalidHelper(
      "{\"\\u0000\":null}", ara::core::configreader::ParseError::kNullUtf16Character, "kNullUtf16Character");

  araLog_.LogVerbose() << "  ({\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":"
                          "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":["
                          "]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]})";
  ParseObjectInvalidHelper(
      "{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":"
      "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":["
      "]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}",
      ara::core::configreader::ParseError::kExpectedNestingLimit, "kExpectedNestingLimit");
}

void CoreApp::IT_Core_0007()
{
  araLog_.LogVerbose() << "###### StartSingleShotTimer_Negative-Test-Cases Started ######";

  araLog_.LogVerbose() << " Invoke GetTimerThreadInstance()";
  timerObj_->GetTimerThreadInstance();
  araLog_.LogVerbose() << "GetTimerThreadInstance() should return singleton object.";

  araLog_.LogVerbose() << " Invoke StartSingleShotTimerWithResult() ";
  ara::core::timer::MilliSeconds timeout{5000};
  ara::core::timer::HandlerType singleShotHandler
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::Result<ara::core::timer::TimerId> id
      = timerObj_->StartSingleShotTimerWithResult(timeout, singleShotHandler);
  araLog_.LogVerbose() << "StartSingleShotTimerWithResult() should return timer id = " << id.Value();

  araLog_.LogVerbose()
      << " Invoke StartSingleShotTimerWithResult() with timeout parameter value is less than 10ms. ";
  ara::core::timer::MilliSeconds timeout1{0};
  ara::core::timer::HandlerType singleShotHandler1
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::Result<ara::core::timer::TimerId> id1
      = timerObj_->StartSingleShotTimerWithResult(timeout1, singleShotHandler1);
  if (id1.Error() == ara::core::TimerErrc::kInvalidReqRequestTimer)
    araLog_.LogVerbose() << "StartSingleShotTimerWithResult() should return error kInvalidReqRequestTimer";

  araLog_.LogVerbose()
      << " Invoke StartSingleShotTimerWithResult() with handler parameter value is equal to null pointer.";
  ara::core::timer::MilliSeconds timeout2{5000};
  ara::core::timer::HandlerType singleShotHandler2 = nullptr;
  ara::core::Result<ara::core::timer::TimerId> id2
      = timerObj_->StartSingleShotTimerWithResult(timeout2, singleShotHandler2);
  if (id2.Error() == ara::core::TimerErrc::kInvalidHandler)
    araLog_.LogVerbose() << "StartSingleShotTimerWithResult() should return error kInvalidHandler";

  araLog_.LogVerbose() << " Invoke StopTimer() ";
  ara::core::timer::TimerId timerid = id.Value();
  ara::core::Result<bool> st = timerObj_->StopTimer(timerid);
  araLog_.LogVerbose() << "StopTimer() should return true = " << st.Value();
}

void CoreApp::IT_Core_0008()
{
  araLog_.LogVerbose() << "###### StartPeriodicTimerWithResult_Negative-Test-Cases Started ######";

  araLog_.LogVerbose() << " Invoke GetTimerThreadInstance()";
  timerObj_->GetTimerThreadInstance();
  araLog_.LogVerbose() << "GetTimerThreadInstance() should return singleton object.";

  araLog_.LogVerbose() << " Invoke StartPeriodicTimerWithResult() ";
  ara::core::timer::MilliSeconds period{5000};
  ara::core::timer::HandlerType startPeriodictHandler
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::Result<ara::core::timer::TimerId> id
      = timerObj_->StartPeriodicTimerWithResult(period, startPeriodictHandler);
  araLog_.LogVerbose() << "StartPeriodicTimerWithResult() should return timer id = " << id.Value();

  araLog_.LogVerbose()
      << " Invoke StartPeriodicTimerWithResult() with timeout parameter value is less than 10ms. ";
  ara::core::timer::MilliSeconds period1{0};
  ara::core::timer::HandlerType startPeriodictHandler1
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::Result<ara::core::timer::TimerId> id1
      = timerObj_->StartPeriodicTimerWithResult(period1, startPeriodictHandler1);
  if (id1.Error() == ara::core::TimerErrc::kInvalidReqRequestTimer)
    araLog_.LogVerbose() << "StartPeriodicTimerWithResult() should return error kInvalidReqRequestTimer";

  araLog_.LogVerbose()
      << " Invoke StartPeriodicTimerWithResult() with handler parameter value is equal to null pointer.";
  ara::core::timer::MilliSeconds period2{5000};
  ara::core::timer::HandlerType startPeriodictHandler2 = nullptr;
  ara::core::Result<ara::core::timer::TimerId> id2
      = timerObj_->StartPeriodicTimerWithResult(period2, startPeriodictHandler2);
  if (id2.Error() == ara::core::TimerErrc::kInvalidHandler)
    araLog_.LogVerbose() << "StartPeriodicTimerWithResult() should return error kInvalidHandler";

  araLog_.LogVerbose() << " Invoke StopTimer() ";
  ara::core::timer::TimerId timerid = id.Value();
  ara::core::Result<bool> st = timerObj_->StopTimer(timerid);
  araLog_.LogVerbose() << "StopTimer() should return true = " << st.Value();
}

void CoreApp::IT_Core_0009()
{
  araLog_.LogVerbose() << "###### GetRemainingTimeWithResult_Negative-Test-Case Started ######";

  araLog_.LogVerbose() << " Invoke GetTimerThreadInstance()";
  timerObj_->GetTimerThreadInstance();
  araLog_.LogVerbose() << "GetTimerThreadInstance() should return singleton object.";

  araLog_.LogVerbose() << " Invoke StartPeriodicTimer() ";
  ara::core::timer::MilliSeconds period{5000};
  ara::core::timer::HandlerType PeriodicHandler
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::timer::TimerId id = timerObj_->StartPeriodicTimer(period, PeriodicHandler);
  araLog_.LogVerbose() << "StartPeriodicTimer() should return timer id = " << id;

  timerObj_->StopTimerWithResult(id);
  araLog_.LogVerbose() << " Invoke GetRemainingTimeWithResult() with timer id parameter is invalid.";
  ara::core::Result<ara::core::timer::MilliSeconds> millsec = timerObj_->GetRemainingTimeWithResult(id);
  if (millsec.Error() == ara::core::TimerErrc::kInvalidId)
    araLog_.LogVerbose() << "GetRemainingTimeWithResult() should return error kInvalidId";
}

void CoreApp::IT_Core_0010()
{
  araLog_.LogVerbose() << "###### StopTimerWithResult_Negative-Test-Case Started ######";

  araLog_.LogVerbose() << " Invoke GetTimerThreadInstance()";
  timerObj_->GetTimerThreadInstance();
  araLog_.LogVerbose() << "GetTimerThreadInstance() should return singleton object.";

  araLog_.LogVerbose() << " Invoke StartPeriodicTimer() ";
  ara::core::timer::MilliSeconds period{5000};
  ara::core::timer::HandlerType PeriodicHandler
      = std::bind(&it::core::application::CoreApp::handler_fun, this, false);
  ara::core::timer::TimerId id = timerObj_->StartPeriodicTimer(period, PeriodicHandler);
  araLog_.LogVerbose() << "StartPeriodicTimer() should return timer id = " << id;

  timerObj_->StopTimerWithResult(id);
  araLog_.LogVerbose() << " Invoke StopTimerWithResult() with timer id parameter is invalid.";
  ara::core::Result<bool> stopTimer = timerObj_->StopTimerWithResult(id);
  if (stopTimer.Error() == ara::core::TimerErrc::kInvalidId)
    araLog_.LogVerbose() << "StopTimerWithResult() should return error kInvalidId";
}

class test_resource : public ara::core::pmr::memory_resource
{
 public:
  explicit test_resource(ara::core::pmr::memory_resource* parent = ara::core::pmr::get_default_resource());
  ~test_resource();

  ara::core::pmr::memory_resource* parent() const;

  size_t bytes_allocated() const;
  size_t bytes_deallocated() const;
  size_t bytes_outstanding() const;
  size_t bytes_highwater() const;
  size_t blocks_outstanding() const;

  static size_t leaked_bytes();
  static size_t leaked_blocks();
  static void clear_leaked();

 protected:
  void* do_allocate(size_t bytes, size_t alignment) override;
  void do_deallocate(void* p, size_t bytes, size_t alignment) override;
  bool do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept override;

 private:
  // Record holding the results of an allocation
  struct allocation_rec
  {
    void* m_ptr;
    size_t m_bytes;
    size_t m_alignment;
  };

  ara::core::pmr::memory_resource* m_parent;
  size_t m_bytes_allocated;
  size_t m_bytes_outstanding;
  size_t m_bytes_highwater;
  ara::core::pmr::Vector<allocation_rec> m_blocks;

  static size_t s_leaked_bytes;
  static size_t s_leaked_blocks;
};

// Keep track of number of bytes that would be leaked by
// allocator destructor.
size_t test_resource::s_leaked_bytes = 0;

// Keep track of number of blocks that would be leaked by
// allocator destructor.
size_t test_resource::s_leaked_blocks = 0;

test_resource::test_resource(ara::core::pmr::memory_resource* parent) :
    m_parent(parent), m_bytes_allocated(0), m_bytes_outstanding(0), m_bytes_highwater(0),
    m_blocks(parent) // Vector memory comes from parent, too
{
}

test_resource::~test_resource()
{
  // If any blocks have not been released, report them as leaked.
  s_leaked_blocks += blocks_outstanding();

  // Reclaim blocks that would have been leaked.
  for (auto& alloc_rec : m_blocks)
  {
    s_leaked_bytes += alloc_rec.m_bytes;
    m_parent->deallocate(alloc_rec.m_ptr, alloc_rec.m_bytes, alloc_rec.m_alignment);
  }
}

ara::core::pmr::memory_resource* test_resource::parent() const
{
  return m_parent;
}

size_t test_resource::bytes_allocated() const
{
  return m_bytes_allocated;
}

size_t test_resource::bytes_deallocated() const
{
  return m_bytes_allocated - m_bytes_outstanding;
}

size_t test_resource::bytes_outstanding() const
{
  return m_bytes_outstanding;
}

size_t test_resource::bytes_highwater() const
{
  return m_bytes_highwater;
}

size_t test_resource::blocks_outstanding() const
{
  return m_blocks.size();
}

size_t test_resource::leaked_bytes()
{
  return s_leaked_bytes;
}

size_t test_resource::leaked_blocks()
{
  return s_leaked_blocks;
}

void test_resource::clear_leaked()
{
  s_leaked_bytes = 0;
  s_leaked_blocks = 0;
}

void* test_resource::do_allocate(size_t bytes, size_t alignment)
{
  void* ret = m_parent->allocate(bytes, alignment);
  m_blocks.push_back(allocation_rec{ret, bytes, alignment});
  m_bytes_allocated += bytes;
  m_bytes_outstanding += bytes;
  if (m_bytes_outstanding > m_bytes_highwater)
    m_bytes_highwater = m_bytes_outstanding;

  return ret;
}

void test_resource::do_deallocate(void* p, size_t bytes, size_t alignment)
{
  // Check that deallocation args exactly match allocation args.
  auto i = std::find_if(m_blocks.begin(), m_blocks.end(), [p](allocation_rec& r) { return r.m_ptr == p; });
  if (i == m_blocks.end())
    throw std::invalid_argument("deallocate: Invalid pointer");
  else if (i->m_bytes != bytes)
    throw std::invalid_argument("deallocate: Size mismatch");
  else if (i->m_alignment != alignment)
    throw std::invalid_argument("deallocate: Alignment mismatch");

  m_parent->deallocate(p, i->m_bytes, i->m_alignment);
  m_blocks.erase(i);
  m_bytes_outstanding -= bytes;
}

bool test_resource::do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept
{
  return this == &other;
}

class debug_resource : public ara::core::pmr::memory_resource
{
 public:
  explicit debug_resource(ara::core::pmr::String name,
      ara::core::pmr::memory_resource* up = ara::core::pmr::get_default_resource()) :
      name_(name),
      upstream_(up)
  {
  }
  inline void* do_allocate(size_t bytes, size_t alignment) override
  {
    std::cout << name_ << " do_allocate(): " << bytes << '\n';
    void* ret = upstream_->allocate(bytes, alignment);
    return ret;
  }
  inline void do_deallocate(void* ptr, size_t bytes, size_t alignment) override
  {
    std::cout << name_ << " do_deallocate(): " << bytes << '\n';
    upstream_->deallocate(ptr, bytes, alignment);
  }
  inline bool do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept override
  {
    return this == &other;
  }

 private:
  ara::core::pmr::memory_resource* upstream_ = ara::core::pmr::get_default_resource();
  ara::core::pmr::String name_;
};
void CoreApp::IT_Core_Pmr_0001()
{
  ara::core::timer::TimerThread& timer{ara::core::timer::TimerThread::GetTimerThreadInstance()};
  ara::core::timer::MilliSeconds timeout{5000};
  ara::core::timer::HandlerType singleShotHandler = std::bind(&CoreApp::pushhandler, this);
  ara::core::timer::TimerId id1 = timer.StartSingleShotTimer(timeout, singleShotHandler);
}

void CoreApp::pushhandler()
{
  araLog_.LogVerbose() << "###### IT_Core_Pmr_0001 monotonic_buffer_resource_TestCases Started ######";
  debug_resource default_dbg("debug_resource");
  int buffer[20];
  std::size_t initial_size{20};
  araLog_.LogVerbose() << " Create ara::core::pmr::monotonic_buffer_resource";
  ara::core::pmr::monotonic_buffer_resource pool{&buffer, initial_size, &default_dbg};
  araLog_.LogVerbose() << "monotonic_buffer_resource should be created.";
  araLog_.LogVerbose() << " Create vector with upstream resource as monotonic_buffer_resource.";
  ara::core::pmr::Vector<int> vec{&pool};
  araLog_.LogVerbose() << "vector should be created.";
  araLog_.LogVerbose() << " Push elements into the vector using push_back().";
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  vec.push_back(5);
  vec.push_back(5);
  vec.push_back(5);
  vec.push_back(5);
  std::cout << "Vector size after push_back" << vec.size() << std::endl;
}
void CoreApp::IT_Core_Pmr_0002()
{
  ara::core::timer::TimerThread& timer{ara::core::timer::TimerThread::GetTimerThreadInstance()};
  ara::core::timer::MilliSeconds period{6000};
  ara::core::timer::HandlerType PeriodicHandler = std::bind(&CoreApp::DefaultUnsyncpmr, this);
  ara::core::timer::TimerId id1 = timer.StartSingleShotTimer(period, PeriodicHandler);
}
void CoreApp::DefaultUnsyncpmr()
{
  araLog_.LogVerbose() << "###### IT_Core_Pmr_0002 unsynchronized_pool_resource Started ######";
  int buffer[1024];
  debug_resource default_dbg("default_resource");
  {
    araLog_.LogVerbose()
        << " Create ara::core::pmr::unsynchronized_pool_resource with upstream resource as monotonic";
    ara::core::pmr::monotonic_buffer_resource monotonic_resource{&buffer, 1024, &default_dbg};
    debug_resource dbg("unsynchronized", &monotonic_resource);
    ara::core::pmr::unsynchronized_pool_resource unsync{&dbg};
    araLog_.LogVerbose() << "unsynchronized_pool_resource should be created.";

    araLog_.LogVerbose() << " Create vector and add entries. ";
    ara::core::pmr::Vector<uint8_t> vec{&unsync};
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    araLog_.LogVerbose() << "vector Size should be " << vec.size();
  }
  std::cout << "allocated buffer" << buffer << std::endl;
}

void CoreApp::IT_Core_Pmr_0003()
{
  ara::core::timer::TimerThread& timer{ara::core::timer::TimerThread::GetTimerThreadInstance()};
  ara::core::timer::MilliSeconds period{5000};
  ara::core::timer::HandlerType PeriodicHandler = std::bind(&CoreApp::NullMemorypmr, this);
  ara::core::timer::TimerId id1 = timer.StartSingleShotTimer(period, PeriodicHandler);
  araLog_.LogVerbose() << "null_memory_resource StartSingleShotTimer() should return timer id = " << id1;
}

void CoreApp::NullMemorypmr()
{
  araLog_.LogVerbose() << "###### IT_Core_Pmr_0003 null_memory_resource Started ######";
  araLog_.LogVerbose() << " Create ara::core::pmr::null_memory_resource.";
  ara::core::pmr::memory_resource* mr = ara::core::pmr::null_memory_resource();
  bool bad_allocexception_thrown = false;
  araLog_.LogVerbose() << "null_memory_resource should be created.";
  araLog_.LogVerbose() << " Inovke allocate of null_memory_resource. ";
  try
  {
    mr->allocate(1, 1);
  }
  catch (std::bad_alloc&)
  {
    bad_allocexception_thrown = true;
  }
  catch (...)
  {
  }
  if (bad_allocexception_thrown == true)
  {
    araLog_.LogVerbose() << "std::bad_alloc is thrown.\n";
  }
}

void CoreApp::IT_Core_Pmr_0004()
{
  ara::core::timer::TimerThread& timer{ara::core::timer::TimerThread::GetTimerThreadInstance()};
  ara::core::timer::MilliSeconds period{5000};
  ara::core::timer::HandlerType PeriodicHandler = std::bind(&CoreApp::NewDeleteMemory, this);
  ara::core::timer::TimerId id1 = timer.StartSingleShotTimer(period, PeriodicHandler);
  araLog_.LogVerbose() << "null_memory_resource StartSingleShotTimer() should return timer id = " << id1;
}
void CoreApp::NewDeleteMemory()
{
  araLog_.LogVerbose() << "###### IT_Core_Pmr_0004 new_delete_resource Started ######";
  araLog_.LogVerbose() << " Create memory resource with ara::core::pmr::new_delete_resource as parent.";
  test_resource NewDelete;
  if (ara::core::pmr::new_delete_resource_singleton() == NewDelete.parent())
  {
    araLog_.LogVerbose() << "new_delete_resource should be the parent resource.";
  }
  if (0 == NewDelete.bytes_allocated())
  {
    araLog_.LogVerbose() << "bytes_allocated should return 0 initially.";
  }
  if (0 == NewDelete.bytes_deallocated())
  {
    araLog_.LogVerbose() << "bytes_deallocated should return 0 initially.";
  }
  if (0 == NewDelete.bytes_outstanding())
  {
    araLog_.LogVerbose() << "bytes_outstanding should return 0 initially.";
  }
  if (0 == NewDelete.bytes_highwater())
  {
    araLog_.LogVerbose() << "bytes_highwater should return 0 initially.";
  }
  if (0 == NewDelete.blocks_outstanding())
  {
    araLog_.LogVerbose() << "blocks_outstanding should return 0 initially.";
  }
  araLog_.LogVerbose() << " Allocate 100 bytes by using allocate().";
  void* p1 = NewDelete.allocate(100);
  if (100 == NewDelete.bytes_allocated())
  {
    araLog_.LogVerbose() << "bytes_allocated should return 100.";
  }
  if (0 == NewDelete.bytes_deallocated())
  {
    araLog_.LogVerbose() << "bytes_deallocated should return 0.";
  }
  if (100 == NewDelete.bytes_outstanding())
  {
    araLog_.LogVerbose() << "bytes_outstanding should return 100.";
  }
  if (100 == NewDelete.bytes_highwater())
  {
    araLog_.LogVerbose() << "bytes_highwater should return 100.";
  }
  if (1 == NewDelete.blocks_outstanding())
  {
    araLog_.LogVerbose() << "blocks_outstanding should return 1.";
  }
  araLog_.LogVerbose() << " deallocate 100 bytes by using deallocate().";
  NewDelete.deallocate(p1, 100);
  if (100 == NewDelete.bytes_allocated())
  {
    araLog_.LogVerbose() << "bytes_allocated should return 100.";
  }
  if (100 == NewDelete.bytes_deallocated())
  {
    araLog_.LogVerbose() << "bytes_deallocated should return 100.";
  }
  if (0 == NewDelete.bytes_outstanding())
  {
    araLog_.LogVerbose() << "bytes_outstanding should return 0.";
  }
  if (100 == NewDelete.bytes_highwater())
  {
    araLog_.LogVerbose() << "bytes_highwater should return 100.";
  }
  if (0 == NewDelete.blocks_outstanding())
  {
    araLog_.LogVerbose() << "blocks_outstanding should return 0.";
  }

  araLog_.LogVerbose() << " Create vector and add entries. ";
  ara::core::pmr::Vector<uint8_t> vec{&NewDelete};
  {
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    araLog_.LogVerbose() << "bytes_allocated: " << NewDelete.bytes_allocated();
    araLog_.LogVerbose() << "bytes_deallocated: " << NewDelete.bytes_deallocated();
  }
  {
    vec.erase(vec.begin());
    araLog_.LogVerbose() << "bytes_allocated: " << NewDelete.bytes_allocated();
    araLog_.LogVerbose() << "bytes_deallocated: " << NewDelete.bytes_deallocated();
  }
  araLog_.LogVerbose() << "Leaked bytes: " << NewDelete.leaked_bytes();
  araLog_.LogVerbose() << "vector Size should be " << vec.size();
}

} // namespace application
} // namespace core
} // namespace it