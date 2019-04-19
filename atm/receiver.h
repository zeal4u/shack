//
// Created by zeal4u on 2019/3/28.
// Author : zeal4u
// Email: jsz1995@live.com
//

#ifndef SHACK_RECEIVER_H
#define SHACK_RECEIVER_H

#include "queue.h"
#include "sender.h"
#include "dispatcher.h"

namespace messaging {

class Receiver {
 public:
  // implicit conversion to Sender
  operator Sender() {
    return Sender(&queue_);
  }

  Dispatcher Wait() {
    return Dispatcher(&queue_);
  }
 private:
  Queue queue_;
};

} // namespace messaging

#endif //SHACK_RECEIVER_H
