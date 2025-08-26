
#include <zal/zal.hpp>

#include <mutex>
#include <vector>
#include <functional>

#define REQUIRE(...)      \
  if (!bool(__VA_ARGS__)) \
  std::exit(-1)

template <int I>
bool b = false;

template <int I>
void f() {
  REQUIRE(!b<I>);
  b<I> = true;
}

struct bad_type {
  bad_type() = default;

  bad_type(bad_type&&) {
    throw std::runtime_error("bad type");
  }
  void operator()() {
    f<10>();
  }
};

int main() {
  on_scope_exit {
    REQUIRE(b<0> && b<1> && b<4> && b<5> && b<6> && b<7> && b<8> && b<9> && b<10>);
  };
  on_scope_exit {
    f<0>();
  };
  on_scope_exit {
    f<1>();
  };
  {
    on_scope_failure(set_bool) {
      f<2>();
    };
  }
  if (!b<2>)
    return 1;
  {
    on_scope_failure(set_bool) {
      f<3>();
    };
    set_bool.no_longer_needed();
  }
  if (b<3>)
    return 2;
  zal::scope_exit no_move{[m = std::mutex()] { f<4>(); }};
  zal::scope_exit from_fn{&f<5>};
  zal::scope_exit from_fn_ref{f<6>};
  zal::scope_failure x([] { f<9>(); });
  REQUIRE(x.failed);
  auto xx = std::move(x);
  REQUIRE(!x.failed && xx.failed);
  std::swap(xx, x);
  REQUIRE(!xx.failed && x.failed);
  std::swap(xx, x);
  REQUIRE(!x.failed && xx.failed);
  x = std::move(xx);
  REQUIRE(x.failed && !xx.failed);
  std::swap(xx, x);
  REQUIRE(!x.failed && xx.failed);
  xx = std::move(x);
  REQUIRE(!xx.failed && x.failed);
  xx = std::move(xx);
  REQUIRE(!xx.failed);
  x = std::move(x);
  REQUIRE(x.failed);

  try {
    zal::scope_failure abc(bad_type{});
    REQUIRE(false);
  } catch (std::runtime_error& e) {
    REQUIRE(std::string_view(e.what()) == "bad type");
  }
  std::vector<zal::scope_failure<std::function<void()>>> wtf;
  wtf.push_back({&f<7>});
  wtf.push_back({f<8>});
  wtf.push_back({[] {}});
  return 0;
}
