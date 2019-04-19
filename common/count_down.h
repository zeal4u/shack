//
// Created by zeal4u on 2019/4/18.
//

#ifndef SHACK_COUNT_DOWN_H
#define SHACK_COUNT_DOWN_H

#include "mutex.h"

#include <condition_variable>
#include <mutex>

class CountDown {
 public:
  explicit CountDown(int count);
  void Wait();
  void GoForIt();

 private:
  std::mutex mu_;
  std::condition_variable con_ GUARDED_BY(mu_);

  int count_ GUARDED_BY(mu_);
};


#endif //SHACK_COUNT_DOWN_H
