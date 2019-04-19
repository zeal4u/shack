//
// Created by zeal4u on 2019/3/26.
//

#ifndef SHACK_PARALLEL_SORT_H
#define SHACK_PARALLEL_SORT_H

#include <list>
#include <future>
#include <atomic>
#include <thread>
#include <memory>
#include <algorithm>
#include <iostream>

#include "safe_stack.h"

template<class T>
void print_list(const std::list<T> &input) 
{
	if (input.empty())
		return;
	std::for_each(input.begin(), input.end(), [](T t){ std::cout<<t<<" "; });
	std::cout<<std::endl;
}

template <class T>
class Sorter{
 private:
  struct ChunkToSort{
    std::list<T> data_;
    std::promise<std::list<T>> promise_;
  };

  safe::LockFreeStack<ChunkToSort*> chunks_;
  std::vector<std::thread> threads_;
  const unsigned max_thread_count_;
  std::atomic<bool> end_of_data_;
	std::mutex mu_;

 public:
  Sorter(): max_thread_count_(std::thread::hardware_concurrency() - 1), end_of_data_(false) {}
	
  ~Sorter() {
    end_of_data_ = true;
    for (unsigned i = 0; i < threads_.size(); ++i) {
			if (threads_[i].joinable())
      	threads_[i].join();
    }
  }

  void try_sort_chunk() {
    std::shared_ptr<ChunkToSort*> chunk = chunks_.pop();
    if (chunk) {
      sort_chunk(chunk);
    }
  }

  std::list<T> do_sort(std::list<T> &chunk_data) 
	{
    if (chunk_data.empty()) {
      return chunk_data;
    }
    std::list<T> result;
    result.splice(result.begin(), chunk_data, chunk_data.begin());
		//print_list(chunk_data);
		//print_list(result);
		if (chunk_data.empty()) 
			return result;
    const T &partition_val = *result.begin();

    typename std::list<T>::iterator divide_point = std::partition(chunk_data.begin(), chunk_data.end(),
        [&](const T &val){return val < partition_val;});
    ChunkToSort new_lower_chunk;
    new_lower_chunk.data_.splice(new_lower_chunk.data_.end(), chunk_data, chunk_data.begin(), divide_point);
    std::future<std::list<T>> new_lower = new_lower_chunk.promise_.get_future();
    chunks_.push(&new_lower_chunk);
    if (threads_.size() < max_thread_count_) {
			// race condition, threads may be competing create new threads;
			std::lock_guard<std::mutex> lk(mu_);
      threads_.push_back(std::thread(&Sorter<T>::sort_thread, this));
    }
		//print_list(chunk_data);
    std::list<T> new_higher(do_sort(chunk_data));
    result.splice(result.end(), new_higher);

    while (new_lower.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
      try_sort_chunk();
    }
    result.splice(result.begin(), new_lower.get());
    return result;
  }

  void sort_chunk(const std::shared_ptr<ChunkToSort*> chunk) {
		//print_list((*chunk)->data_);
    (*chunk)->promise_.set_value(do_sort((*chunk)->data_));
  }

  void sort_thread() {
    while (!end_of_data_) {
      try_sort_chunk();
      std::this_thread::yield();
    }
  }
};

template <class T>
std::list<T> parallel_quick_sort(std::list<T> input) 
{
  if (input.empty()) {
    return input;
  }
  Sorter<T> s;
  return s.do_sort(input);
}

#endif //SHACK_PARALLEL_SORT_H
