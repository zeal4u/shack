//
// Created by zeal4u on 2019/3/28.
// Author : zeal4u
// Email : jsz1995@live.com
//

#ifndef SHACK_DISPATCHER_H
#define SHACK_DISPATCHER_H

#include "queue.h"
#include "template_dispatcher.h"
#include <utility>

namespace messaging {

class CloseQueue {};

class Dispatcher {
 public:
  Dispatcher(Dispatcher &&other) : queue_ptr_(other.queue_ptr_), chained_(other.chained_) {
    other.chained_ = true;
  }

  explicit Dispatcher(Queue *queue) : queue_ptr_(queue), chained_(false) {}

  template <class Message, class Func>
  TemplateDispatcher<Dispatcher, Message, Func> Handle(Func &&f) {
    return TemplateDispatcher<Dispatcher, Message, Func>(queue_ptr_, this, std::forward<Func>(f));
  }

  ~Dispatcher() noexcept(false) {
    if (!chained_) {
      WaitAndDispatch();
    }
  }
 private:
  Queue *queue_ptr_;
  bool chained_;

  Dispatcher(const Dispatcher &) = delete;
  Dispatcher& operator=(const Dispatcher &) = delete;

  template <class Dispatcher, class Msg, class Func>
  friend class TemplateDispatcher;

  void WaitAndDispatch() {
    while (true) {
      auto msg = queue_ptr_->WaitAndPop();
      Dispatch(msg);
    }
  }

  bool Dispatch(const std::shared_ptr<MessageBase> &msg) {
    if (dynamic_cast<WrappedMessage<CloseQueue>*> (msg.get())) {
      throw CloseQueue();
    }
    return false;
  }
};

} // namespace messaging

#endif //SHACK_DISPATCHER_H
