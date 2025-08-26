#pragma once

#include <type_traits>
#include <utility>

#if __cplusplus >= 202002L
#define LOGIC_GUARDS_CONSTEXPR_DCTOR constexpr
#else
#define LOGIC_GUARDS_CONSTEXPR_DCTOR
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(no_unique_address)
#define ZAL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
#define ZAL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define ZAL_NO_UNIQUE_ADDRESS
#endif
#else
#define ZAL_NO_UNIQUE_ADDRESS
#endif

namespace zal::noexport {
// forbids move for type without breaking 'is_aggregate'
struct pin {
  pin() = default;
  pin(pin&&) = delete;
  void operator=(pin&&) = delete;
};

}  // namespace zal::noexport

// must be used as field in type, makes it unmovable without breaking
// is_aggregate
#define ZAL_PIN ZAL_NO_UNIQUE_ADDRESS ::zal::noexport::pin _pin_ = {}

// both scope_exit and scope_failure use precondition,
// that invoking 'fn' do not dellocates memory where 'scope_exit/failure' exists
// or atleast do not touch memory after deleting(including lambda capture/references to other objects)

namespace zal {

// SCOPE EXIT
// invokes 'Foo' when destroyed
// if scope ended with exception and 'Foo' throws exception terminate is called
template <typename Foo>
struct [[nodiscard("name it")]] scope_exit {
  ZAL_NO_UNIQUE_ADDRESS Foo fn;
  ZAL_PIN;

  LOGIC_GUARDS_CONSTEXPR_DCTOR
  ~scope_exit() noexcept(noexcept(fn())) {
    (void)fn();
  }
};
template <typename T>
scope_exit(T&&) -> scope_exit<std::decay_t<T>>;

struct movebool {
  bool value;

  movebool() = default;
  movebool(bool b) noexcept : value(b) {
  }
  movebool(movebool const& other) = default;
  movebool& operator=(movebool const& other) = default;
  movebool(movebool&& other) noexcept : value(std::exchange(other.value, false)) {
  }
  movebool& operator=(movebool&& other) noexcept {
    std::swap(value, other.value);
    return *this;
  }

  operator bool() const noexcept {
    return value;
  }
};

// ON SCOPE FAILURE
// invokes 'Foo' when destroyed and success point not reached
// success point setted by invoking .no_longer_needed()
// may be moved, guarantees `fn` will be invoked once
template <typename Foo>
struct [[nodiscard("name it and set success points")]] scope_failure {
  ZAL_NO_UNIQUE_ADDRESS Foo fn;
  movebool failed = true;

  scope_failure(Foo fn) noexcept
    requires(std::is_nothrow_move_constructible_v<Foo>)
      : fn(std::move(fn)) {
  }
  scope_failure(Foo fn)
    requires(!std::is_nothrow_move_constructible_v<Foo>)
  try : fn(std::move(fn)) {
  } catch (...) {
    fn();
    throw;
  }
  scope_failure(scope_failure&&) = default;
  scope_failure& operator=(scope_failure&&) = default;

  LOGIC_GUARDS_CONSTEXPR_DCTOR
  ~scope_failure() noexcept(noexcept(fn())) {
    if (failed)
      (void)fn();
  }
  // should be called where the 'fn' call is no longer needed
  constexpr void no_longer_needed() noexcept {
    failed = false;
  }
  constexpr void use_and_forget() noexcept(noexcept(fn())) {
    failed = false;
    (void)fn();
  }
};
template <typename T>
scope_failure(T&&) -> scope_failure<std::decay_t<T>>;

}  // namespace zal

// MACROS on_scope_exit / on_scope_failure(NAME) for easy usage

namespace zal::noexport {

// used only in macro
struct maker {
  template <typename T>
  constexpr auto operator+(T&& fn) const noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T&&>) {
    return scope_exit{std::forward<T>(fn)};
  }
  template <typename T>
  constexpr auto operator-(T&& fn) const noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T&&>) {
    return scope_failure{std::forward<T>(fn)};
  }
};

}  // namespace zal::noexport

// code after this macro will be executed on scope exit
#define on_scope_exit                                                                  \
  [[maybe_unused]] const auto LOGIC_GUARDS_CONCAT(scope_exit, __LINE__, __COUNTER__) = \
      ::zal::noexport::maker{} + [&]()

// user of this macro should use NAME.no_longer_needed() to not execute guard on scope exit
#define on_scope_failure(NAME) auto NAME = ::zal::noexport::maker{} - [&]()

// generates unique name from __LINE__ and __COUNTER__
#define LOGIC_GUARDS_CONCAT_IMPL_EXPAND(a, b, c) a##_##b##_##c
#define LOGIC_GUARDS_CONCAT_IMPL(a, b, c) LOGIC_GUARDS_CONCAT_IMPL_EXPAND(a, b, c)
#define LOGIC_GUARDS_CONCAT(a, b, c) LOGIC_GUARDS_CONCAT_IMPL(a, b, c)
