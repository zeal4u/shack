#include <iostream>

int main() {
  const int a = 10;
  int *p = const_cast<int*>(&a);
  *p = 20;
  std::cout<<"a = "<<a<<", *p = "<<*p<<std::endl;
  return 0;
}
