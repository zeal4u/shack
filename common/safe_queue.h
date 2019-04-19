#ifndef _SAFE_QUEUE_H_
#define _SAFE_QUEUE_H_

#include <memory>
#include <mutex>
#include <condition_variable>

namespace safe{

template<class T>
class SafeQueue {
 private:
  struct Node{
    std::shared_ptr<T> data;
    std::unique_ptr<Node> next;
  };

  // 按定义顺序初始化
  std::mutex head_mutex_;
  std::unique_ptr<Node> head_;
  std::mutex tail_mutex_;
  Node *tail_;
  std::condition_variable data_cond_;

  Node *get_tail()
  { 
    std::lock_guard<std::mutex> tail_lock(tail_mutex_);
    return tail_;
  }

  std::unique_ptr<Node> pop_head()
  {
    std::unique_ptr<Node> old_head = std::move(head_);
    head_ = std::move(old_head->next);
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data()
  {
    std::unique_lock<std::mutex> head_lock(head_mutex_);
    data_cond_.wait(head_lock, [this](){ return head_.get() != get_tail();});
    return std::move(head_lock);
  }

  std::unique_ptr<Node> wait_pop_head()
  {
    std::unique_lock<std::mutex> head_lock(wait_for_data()); 
    return pop_head();
  }

  std::unique_ptr<Node> wait_pop_head(T &value)
  {
    std::unique_lock<std::mutex> head_lock(wait_for_data()); 
    value = std::move(*head_->data);
    return pop_head();
  }

  std::unique_ptr<Node> try_pop_head()
  {
    std::lock_guard<std::mutex> head_lock(head_mutex_);
    if (head_.get() == get_tail()) {
      return std::unique_ptr<Node>();
    } else {
      return pop_head(); 
    }
  }

  std::unique_ptr<Node> try_pop_head(T &value)
  {
    std::lock_guard<std::mutex> head_lock(head_mutex_);
    if (head_.get() == get_tail()) {
      return std::unique_ptr<Node>();
    } else {
      value = std::move(*head_->data);
      return pop_head(); 
    }
  }

 public:
  SafeQueue(): head_(new Node), tail_(head_.get()) {}
  SafeQueue(const SafeQueue&) = delete;
  SafeQueue& operator=(const SafeQueue&) = delete;

  std::shared_ptr<T> try_pop();
  bool try_pop(T &value);
  std::shared_ptr<T> wait_and_pop();
  void wait_and_pop(T &value);
  void push(T value);
  void empty();
};

template<class T>
void SafeQueue<T>::push(T value)
{
  std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
  std::unique_ptr<Node> p(new Node);
  {
    std::lock_guard<std::mutex> tail_lock(tail_mutex_);
    tail_->data = new_data;
    Node *new_tail = p.get();
    tail_->next = std::move(p);
    tail_ = new_tail;
  }
  data_cond_.notify_one();
}

template<class T>
std::shared_ptr<T> SafeQueue<T>::wait_and_pop()
{
  std::unique_ptr<Node> const old_head = wait_pop_head();
  return old_head->data;
}

template<class T>
void SafeQueue<T>::wait_and_pop(T &value)
{
  std::unique_ptr<Node> const old_head = wait_pop_head(value);
}

template<class T>
std::shared_ptr<T> SafeQueue<T>::try_pop()
{
  std::unique_ptr<Node> old_head = try_pop_head();
  return old_head;
}

template<class T>
bool SafeQueue<T>::try_pop(T &value)
{
  std::unique_ptr<Node> old_head = try_pop_head(value);
  return old_head;
}

template<class T>
void SafeQueue<T>::empty()
{
  std::lock_guard<std::mutex> head_lock(head_mutex_);
  return (head_.get() == get_tail());
}
} // namespace safe
#endif //_SAFE_QUEUE_H_
