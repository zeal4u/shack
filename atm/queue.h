//
// Created by zeal4u on 2019/3/28.
// Author: zeal4u
// Email: jsz1995@live.com
//

#ifndef SHACK_QUEUE_H
#define SHACK_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace messaging {

struct MessageBase{
  virtual ~MessageBase() {}
};

template <class Msg>
struct WrappedMessage : MessageBase {
  Msg contents_;
  explicit WrappedMessage(const Msg& contents) : contents_(contents) {}
};

class Queue {
 public:
  template<class T>
  void Push(const T& msg) {
    std::lock_guard<std::mutex> lk(mutex_);
    queue_.push(std::make_shared<WrappedMessage<T>>(msg));
    cond_var_.notify_all();
  }

  std::shared_ptr<MessageBase> WaitAndPop() {
    std::unique_lock<std::mutex> lk(mutex_);
    cond_var_.wait(lk, [&]{return !queue_.empty();});
    auto res = queue_.front();
    queue_.pop();
    return res;
  }

 private:
  std::mutex mutex_;
  std::condition_variable cond_var_;
  std::queue<std::shared_ptr<MessageBase>> queue_;
};
} //namespace messaging
#endif //SHACK_QUEUE_H
