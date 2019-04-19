#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <atomic>
#include <chrono>
#include <thread>

#include "parallel_accumulate.h"
#include "count_down.h"

int critical_value = 0;
std::atomic<int> atomic_value(0);

void* add_process(void* mutex)
{
  int count = 10000;
  while (count--) {
    pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(mutex));
    critical_value++;
    //printf("Thread ID: %lu,  value %d\n", static_cast<unsigned long>(pthread_self()), critical_value);
    pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(mutex));
  }
  return nullptr;
}

void* atomic_add(void* _)
{
  int count = 10000;
  while (count--)
    atomic_value++; 
  return nullptr;
}

void test_multi_add(int argc, char* argv[])
{
  if (argc != 2) return;
  int i = 10;
  pthread_t thread[i];
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, nullptr);
  if (strcmp("atomic", argv[1]) == 0) {
    for (int j = 0; j < i; ++j) {
      memset(&thread[j], 0, sizeof(pthread_t));
      pthread_create(&thread[j], nullptr, atomic_add, nullptr);
    }
      
  } else {
    for (int j = 0; j < i; ++j) {
      memset(&thread[j], 0, sizeof(pthread_t));
      pthread_create(&thread[j], nullptr, add_process, &mutex);
    }
  }

  void* tret = nullptr;
  for (int j = 0; j < i; ++j) {
    pthread_join(thread[j], &tret);
  }
 
  printf("Finally the value is %d\n", (critical_value != 0 ? critical_value: std::atomic_load(&atomic_value)));
}

void test_parallel_accumulate()
{
  std::vector<int> test_data = {1,2,3,4};
  auto start = std::chrono::high_resolution_clock::now();
  int result = parallel_accumulate(test_data.begin(), test_data.end(), 0);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  printf("Parallel accumulate result is %d, cost %f s\n", result, diff.count());


  start = std::chrono::high_resolution_clock::now();
  result = accumulate(test_data.begin(), test_data.end(), 0);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;
  printf("Correct accumulate result is %d, cost %f s\n", result, diff.count());
}

int normal_int = 0;

void LatchAdd(CountDown *cd) {
  cd->Wait();
  ++normal_int;
  printf("%d", normal_int);
}

void test_latchadd() {
  normal_int = 0;
  CountDown cd(2);
  std::thread thread1(LatchAdd, &cd);
  cd.GoForIt();
  std::thread thread2(LatchAdd, &cd);
  cd.GoForIt();
  thread1.join();
  thread2.join();
  printf("\n");
}

int main(int argc, char *argv[])
{
  int times = 100000;
  while (times--) {
    test_latchadd();
  }
  return 0;
}
