//
// Created by zeal4u on 2019/3/28.
// Author: zeal4u
// Email: jsz1995@live.com
//

#ifndef SHACK_SENDER_H
#define SHACK_SENDER_H

#include "queue.h"

namespace messaging {

class Sender {
 public:
  Sender() : queue_ptr_(nullptr) {}
  explicit Sender(Queue *queue_ptr) : queue_ptr_(queue_ptr) {}

  template <class Message>
  void Send(const Message &msg) {
    if (queue_ptr_) {
      queue_ptr_->Push(msg);
    }
  }
 private:
  Queue *queue_ptr_;
};

} // namespace messaging

#endif //SHACK_SENDER_H
