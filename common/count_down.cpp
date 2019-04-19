//
// Created by zeal4u on 2019/4/18.
//

#include "count_down.h"

CountDown::CountDown(int count) : count_(count) {}

void CountDown::Wait() {
  std::unique_lock<std::mutex> lk(mu_);
  con_.wait(lk, [this](){ return count_ == 0;});
}

void CountDown::GoForIt() {
  std::lock_guard<std::mutex> lk(mu_);
  --count_;
  if (count_ == 0) {
    con_.notify_all();
  }
}
