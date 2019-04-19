//
// Created by zeal4u on 2019/3/26.
//

#ifndef SHACK_JOIN_THREAD_H
#define SHACK_JOIN_THREAD_H

#include <thread>
class JoinThread {
 private:
  std::thread &thread_;
 public:
  explicit JoinThread(std::thread &thread): thread_(thread) {}
  JoinThread(const JoinThread&) = delete;
  JoinThread& operator=(JoinThread&) = delete;

  ~JoinThread() {
    if (thread_.joinable()) {
      thread_.join();
    }
  }

};


#endif //SHACK_JOIN_THREAD_H
