//
// Created by zeal4u on 2019/4/3.
//

#ifndef SHACK_LRU_HASH_MAP_H
#define SHACK_LRU_HASH_MAP_H

#include "traits.h"

#include <utility>
#include <unordered_map>
#include <list>
#include <chrono>
#include <assert.h>

template<class Key, class Value>
class LRUHashMap {
 public:
  typedef typename TypeTraits<Key>::ValueType KeyValueType;
  typedef typename TypeTraits<Key>::Pointer KeyPointer;
  typedef typename TypeTraits<Key>::Reference KeyReference;

  typedef typename TypeTraits<Value>::ValueType ValueValueType;
  typedef typename TypeTraits<Value>::Pointer ValuePointer;
  typedef typename TypeTraits<Value>::Reference ValueReference;

  typedef typename std::list<KeyValueType>::iterator OrderPtr; 

  typedef std::pair<KeyValueType, ValueValueType> DataType;

  ValueValueType Get(KeyValueType key) { 
    if (!Contains(key))
      assert(0);
    return records_[key].data_;
  }

  void Set(KeyValueType key, ValueValueType val) {
    __Value tmp;
    
    if (Contains(key)) {
      tmp = records_[key];
      order_.erase(tmp.order_ptr_);
    }

    order_.push_back(key);

    tmp.data_ = val;
    tmp.timestamp_ = timestamp(); 
    tmp.order_ptr_ = --order_.end();
    records_[key] = tmp;
  }

  void Set(DataType data) {
    Set(data.first, data.second);
  }

  void Delete(KeyValueType key) {
    // TODO 
  }

  bool Contains(KeyValueType key) {
    return records_.find(key) != records_.end();
  }

  void RetireByTime(unsigned long long time) {
    for (auto item = order_.begin(); item != order_.end();) {
      __Value tmp = records_[*item];
      if (time < tmp.timestamp_)
        break;
      records_.erase(*item);
      item = order_.erase(item); 
    }
  }

  void RetireNum(int num) {
    for (auto item = order_.begin(); item != order_.end() && num;) {
      num--;
      records_.erase(*item);
      item = order_.erase(item);
    }
  }

 private:
  struct __Value {
    ValueValueType data_;
    OrderPtr order_ptr_;
    unsigned long long timestamp_; 
  };

  unsigned long long timestamp() {
    return std::chrono::system_clock::now().time_since_epoch().count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
  }

  std::list<KeyValueType> order_;
  std::unordered_map<KeyValueType, __Value> records_;
};

#endif //SHACK_LRU_HASH_MAP_H
