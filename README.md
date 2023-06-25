# logic_guards
I'm tired of seeing poorly written scope exits in many projects, so i create header only library with scope_exit and scope_failure designed to be copy-pasted into any other project

Usage:

```C++

auto usage_example1() {
  T* p = new T;
  on_scope_exit {
    delete p;
  };
  // ...
}

T* usage_example2() {
  T* p = new T;
  on_scope_failure(free_memory) {
    delete p;
  };
  // ...

  // here we show that 'dangerous' operations are completed
  // and we dont need free memory on exit
  free_memory.no_longer_needed();
  return p;
}

```
