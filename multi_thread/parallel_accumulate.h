//
// Created by zeal4u on 2019/3/26.
//

#ifndef SHACK_PARALLEL_ACCUMULATE_H
#define SHACK_PARALLEL_ACCUMULATE_H

#include <iterator>
#include <future>
#include <algorithm>

template <class Iterator, class T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
  const unsigned long length = std::distance(first, last);
  const unsigned long max_chunk_size = 25;
  if (length <= max_chunk_size) {
    return std::accumulate(first, last, init);
  } else {
    Iterator mid_point = first;
    std::advance(mid_point, length/2);
    std::future<T> first_half_result = std::async(std::launch::async, parallel_accumulate<Iterator, T>, first, mid_point, init);
    T second_half_result = parallel_accumulate(mid_point, last, T());
    return first_half_result.get() + second_half_result;
  }
}
#endif //SHACK_PARALLEL_ACCUMULATE_H
