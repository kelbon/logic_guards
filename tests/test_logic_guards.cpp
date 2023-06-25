
#include "logic_guards/logic_guards.hpp"

#include <mutex>

template <int I>
bool b = false;

template <int I>
void f() {
  b<I> = true;
}

int main() {
  on_scope_exit {
    if (!b<0> || !b<1> || !b<4> || !b<5> || !b<6>)
      exit(3);
  };
  on_scope_exit {
    f<0>();
  };
  on_scope_exit {
    f<1>();
  };
  {
    aa::on_scope_failure set_bool = [] { f<2>(); };
  }
  if (!b<2>)
    return 1;
  {
    aa::on_scope_failure set_bool = [] { f<3>(); };
    set_bool.success_point();
  }
  if (b<3>)
    return 2;
  aa::scope_exit no_move {[m = std::mutex()] {
    f<4>();
  }};
  aa::scope_exit from_fn{&f<5>};
  aa::scope_exit from_fn_ref{f<6>};
  return 0;
}