//
// Created by zeal4u on 2019/3/25.
//

#ifndef LEETCODE_SAFE_STACK_H
#define LEETCODE_SAFE_STACK_H


#include <memory>
#include <atomic>

namespace safe{

template <class T>
class SafeStack{
 private:
  struct Node{
    std::shared_ptr<T> data_;
    std::shared_ptr<Node> next_;

    Node(const T& data): data_(std::make_shared<T>(data)) {}
  };

  std::shared_ptr<Node> head_;
 public:
  void push(const T& data) {
    std::shared_ptr<Node> new_node = std::make_shared<Node>(data);
    new_node->next = head_.get();
		// TODO don't have this function
    while (!std::atomic_compare_exchange_weak(&head_, &new_node->next_, new_node));
  }

  std::shared_ptr<T> pop() {
    std::shared_ptr<Node> old_head = head_.get();
		// TODO don't have this function
    while (old_head && !std::atomic_compare_exchange_weak(&head_, &old_head, old_head->next_));
    return old_head ? old_head->data_ : std::shared_ptr<T>();
  }
};

template <class T>
class LockFreeStack{
 private:
  struct Node;

  class CountedNodePtr{
	 private:
		unsigned long store_ = 0;
	 public:
		const static unsigned long MASK = (1LL << 48) - 1;

		Node *get_ptr() 
		{
			return reinterpret_cast<Node*>(store_ & MASK);	
		}

		void set_ptr(Node *node_ptr)
		{
			unsigned long node_ptr_val = reinterpret_cast<unsigned long>(node_ptr);
			store_ &= ~MASK;
			store_ |= node_ptr_val;
		}

		unsigned long get_external_count()
		{
			return store_ >> 48;
		}
		void set_external_count(unsigned long external_count)
		{
			// TODO what if external_count is larger than 16bit?
			store_ &= MASK;
			store_ |= (external_count << 48);
		}
  };

  struct Node{
    std::shared_ptr<T> data_;
    std::atomic<int> internal_count_;
    CountedNodePtr next_;

    Node(const T& data): 
			data_(std::make_shared<T>(data)), 
			internal_count_(0) {}
  };

  std::atomic<CountedNodePtr> head_;

  void increase_head_count(CountedNodePtr &old_counter) {
    CountedNodePtr new_counter;
    do {
      new_counter = old_counter;
			unsigned long external_count = new_counter.get_external_count();
			new_counter.set_external_count(external_count + 1);
    }
    while (!head_.compare_exchange_strong(old_counter, new_counter,
																					std::memory_order_acquire,
																					std::memory_order_relaxed));
    old_counter.set_external_count(new_counter.get_external_count());
  }

 public:
  ~LockFreeStack() {
    while(pop());
  }

  void push(const T& data) {
    CountedNodePtr new_node;
		Node* node = new Node(data);
    new_node.set_ptr(node);
    new_node.set_external_count(1);
    new_node.get_ptr()->next_ = head_.load(std::memory_order_relaxed);
    while (!head_.compare_exchange_weak(new_node.get_ptr()->next_, new_node,
																				std::memory_order_release,
																				std::memory_order_relaxed));
  }

  std::shared_ptr<T> pop() {
    CountedNodePtr old_head = head_.load(std::memory_order_relaxed);
    while (true) {
      increase_head_count(old_head);
      Node *const ptr = old_head.get_ptr();
      if (!ptr) {
        return std::shared_ptr<T>();
      }
      if (head_.compare_exchange_strong(old_head, ptr->next_,
																				std::memory_order_relaxed)) {
        std::shared_ptr<T> res;
        res.swap(ptr->data_);

        const int count_increase = old_head.get_external_count() - 2;

        if (ptr->internal_count_.fetch_add(count_increase,
							std::memory_order_release) == -count_increase) {
          delete ptr;
        }
        return res;
      } else if (ptr->internal_count_.fetch_sub(1, 
						std::memory_order_relaxed) == 1) {
        delete ptr;
      }
    }
  }
};

} // namespace safe
#endif //LEETCODE_SAFE_STACK_H
