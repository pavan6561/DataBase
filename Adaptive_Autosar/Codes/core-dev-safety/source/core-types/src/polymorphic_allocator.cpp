// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : polymorphic_allocator.cpp
//
// Purpose     : Contains the implementation of the Polymorphic allocator class.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/polymorphic_allocator.h"

std::atomic<ara::core::pmr::memory_resource*> ara::core::pmr::memory_resource::s_default_resource(nullptr);

// T must be: no-throw default constructible and no-throw destructible
template <typename T>
struct singleton_default
{
 private:
  struct object_creator
  {
    // This constructor does nothing more than ensure that instance()
    // is called before main() begins, thus creating the static
    // T object before multithreading race issues can come up.
    object_creator()
    {
      singleton_default<T>::instance();
    }
    inline void do_nothing() const
    {
    }
  };
  static object_creator create_object;

  singleton_default();

 public:
  typedef T object_type;

  // If, at any point (in user code), singleton_default<T>::instance()
  // is called, then the following function is instantiated.
  static object_type& instance()
  {
    // This is the object that we return a reference to.
    // It is guaranteed to be created before main() begins because of
    // the next line.
    static object_type obj;

    // The following line does nothing else than force the instantiation
    // of singleton_default<T>::create_object, whose constructor is
    // called before main() begins.
    create_object.do_nothing();

    return obj;
  }
};
template <typename T>
typename singleton_default<T>::object_creator singleton_default<T>::create_object;

namespace ara
{
namespace core
{
namespace pmr
{
memory_resource* null_memory_resource() noexcept
{
  return &singleton_default<null_memory_resource_imp>::instance();
}

new_delete_resource* new_delete_resource_singleton()
{
  // TBD: I think the standard makes this exception-safe, otherwise, we need
  // to use 'call_once()' in '<mutex>'.
  static new_delete_resource singleton;
  return &singleton;
}
} // namespace pmr
} // namespace core
} // namespace ara

// end polymorphic_allocator.cpp
