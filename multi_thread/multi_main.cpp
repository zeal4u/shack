#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <atomic>

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

int main(int argc, char* argv[])
{
  if (argc != 2) return 0;
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
  return 0;
}
