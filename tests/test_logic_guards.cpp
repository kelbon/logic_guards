
#include "logic_guards/logic_guards.hpp"

#include <mutex>
#include <vector>
#include <functional>

template <int I>
bool b = false;

template <int I>
void f() {
  b<I> = true;
}

int main() {
  on_scope_exit {
    if (b<0> && b<1> && b<4> && b<5> && b<6> && b<7> && b<8>)
      return;
    exit(3);
  };
  on_scope_exit {
    f<0>();
  };
  on_scope_exit {
    f<1>();
  };
  {
    on_scope_failure(set_bool) { f<2>(); };
  }
  if (!b<2>)
    return 1;
  {
    on_scope_failure(set_bool) { f<3>(); };
    set_bool.no_longer_needed();
  }
  if (b<3>)
    return 2;
  aa::scope_exit no_move{[m = std::mutex()] { f<4>(); }};
  aa::scope_exit from_fn{&f<5>};
  aa::scope_exit from_fn_ref{f<6>};
  std::vector<aa::scope_exit<std::function<void()>>> wtf{
      {&f<7>},
      {f<8>},
      {[] {}},
  };
  (void)wtf;
  return 0;
}