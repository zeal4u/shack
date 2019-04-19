#ifndef _MIN_STACK_H_
#define _MIN_STACK_H_

#include <stack>

template<class T>
class MinStack{
public:
  void push(int value) {
    int min = value;
    if (!mins.empty() && value > mins.top())
      min = mins.top();
    mins.push(min);
    values.push(value);
  }
  void pop() {
    if (values.empty())
      return;
    values.pop();
    mins.pop();
  }
  int top() {
    return values.top();
  }
  int min() {
    return mins.top();
  }
private:
  std::stack<T> values;
  std::stack<T> mins;
};
#endif //_MIN_STACK_H_
