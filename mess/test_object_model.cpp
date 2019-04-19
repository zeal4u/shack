#include <stdio.h>

class Base {
 private:
  int base;
};

class A : public Base {
 public:
  int a;
};

class B : public Base {
 public:
  int b;
};

class Foo : public A, public B {
 public:
  virtual void VirtualFooFunc0() {
    printf("Foo::VirtualFooFunc, nonstatic member function is inovked\n");
  }
  virtual void VirtualFooFunc() {
    printf("Foo::VirtualFooFunc, nonstatic member function is inovked\n");
  }

  void FooFunc() {
    printf("Foo::FooFunc, nonstatic member function is inovked\n");
  }

  int foo;
};

int main() {
  Foo f;
  void (Foo::*foo_func)() = &Foo::FooFunc;
  void (Foo::*vfoo_func)() = &Foo::VirtualFooFunc;
  (f.*foo_func)();
  (f.*vfoo_func)();
  printf("0x%llx, 0x%llx\n", &Foo::FooFunc, &Foo::VirtualFooFunc);
  return 0;
}
