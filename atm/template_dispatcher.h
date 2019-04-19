//
// Created by zeal4u on 2019/3/28.
// Author : zeal4u
// Email : jsz1995@live.com

#ifndef SHACK_TEMPLATE_DISPATCHER_H
#define SHACK_TEMPLATE_DISPATCHER_H

#include "queue.h"

namespace messaging {

template <class PreviousDispatcher, class Msg, class Func>
class TemplateDispatcher {
 public:
  TemplateDispatcher(TemplateDispatcher &&other)
      : queue_ptr_(other.queue_ptr_),
        func_(other.func_),
        chained_(other.chained_) {
    other.chained_ = true;
  }

  TemplateDispatcher(Queue *queue, PreviousDispatcher *prev_ptr, Func &&func)
      : queue_ptr_(queue),
        func_(std::forward<Func>(func)),
        prev_ptr_(prev_ptr),
        chained_(false) {
    prev_ptr_->chained_ = true;
  }

  TemplateDispatcher(const TemplateDispatcher &) = delete;
  TemplateDispatcher& operator=(const TemplateDispatcher &) = delete;

  template <class OtherMsg, class OtherFunc>
  TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> Handle(OtherFunc &&other_func) {
    return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
        (queue_ptr_, this, std::forward<OtherFunc>(other_func));
  }

  ~TemplateDispatcher() noexcept(false) {
    if (!chained_) {
      WaitAndDispatch();
    }
  }
 private:
  Queue *queue_ptr_;
  PreviousDispatcher *prev_ptr_;
  Func func_;
  bool chained_;

  // TemplateDispatcher instances are friends between each other;
  template <class Dispatcher, class OtherMsg, class OtherFunc>
  friend class TemplateDispatcher;

  void WaitAndDispatch() {
    while (true) {
      auto msg = queue_ptr_->WaitAndPop();
      if (Dispatch(msg))
        break;
    }
  }

  bool Dispatch(const std::shared_ptr<MessageBase> &msg) {
    if (WrappedMessage<Msg> *wrapper = dynamic_cast<WrappedMessage<Msg>*>(msg.get())) {
      func_(wrapper->contents_);
      return true;
    } else {
      return prev_ptr_->Dispatch(msg);
    }
  }
};
} // namespace messaging

#endif //SHACK_TEMPLATE_DISPATCHER_H
