#pragma once

#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#endif

#include <type_traits>
#include <utility>

#if __cplusplus >= 202002L
#define LOGIC_GUARDS_CONSTEXPR_DCTOR constexpr
#else
#define LOGIC_GUARDS_CONSTEXPR_DCTOR
#endif

namespace aa {

// SCOPE EXIT
// invokes 'Foo' when destroyed
// if scope ended with exception and 'Foo' throws exception terminate is called
template <typename Foo>
struct [[nodiscard("name it")]] scope_exit {
  [[no_unique_address]] Foo fn;

  LOGIC_GUARDS_CONSTEXPR_DCTOR
  ~scope_exit() noexcept(noexcept(fn())) {
    fn();
  }
};
template <typename T>
scope_exit(T&&) -> scope_exit<std::decay_t<T>>;

// ON SCOPE FAILURE
// invokes 'Foo' when destroyed and success point not reached
// success point setted by invoking .no_longer_needed()
template <typename Foo>
struct [[nodiscard("name it and set success points")]] on_scope_failure {
  [[no_unique_address]] Foo fn;
  bool failed = true;

  template <typename... Args>
  constexpr on_scope_failure(Args && ... args) : fn(std::forward<Args>(args)...), failed(true) {
  }

  template <typename T>
  void operator=(T&&) = delete;

  LOGIC_GUARDS_CONSTEXPR_DCTOR
  ~on_scope_failure() noexcept(noexcept(fn())) {
    if (failed)
      fn();
  }
  // should be called where the 'fn' call is no longer needed
  constexpr void no_longer_needed() noexcept {
    failed = false;
  }
};
template <typename T>
on_scope_failure(T&&) -> on_scope_failure<std::decay_t<T>>;

}  // namespace aa

// MACRO on_scope_exit for easy usage

namespace aa::noexport {

// used only in macro
struct make_scope_exit {
  template <typename T>
  constexpr auto operator+(T&& fn) const noexcept(std::is_nothrow_constructible_v<T, T&&>) {
    return scope_exit{std::forward<T>(fn)};
  }
};

}  // namespace aa::noexport

#define on_scope_exit                                                                  \
  [[maybe_unused]] const auto LOGIC_GUARDS_CONCAT(scope_exit, __LINE__, __COUNTER__) = \
      ::aa::noexport::make_scope_exit{} + [&]()

#define LOGIC_GUARDS_CONCAT_IMPL_EXPAND(a, b, c) a##_##b##_##c
#define LOGIC_GUARDS_CONCAT_IMPL(a, b, c) LOGIC_GUARDS_CONCAT_IMPL_EXPAND(a, b, c)
#define LOGIC_GUARDS_CONCAT(a, b, c) LOGIC_GUARDS_CONCAT_IMPL(a, b, c)

#if __clang__
#pragma clang diagnostic pop
#endif
