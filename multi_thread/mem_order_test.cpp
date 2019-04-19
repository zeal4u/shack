#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
  x.store(true, std::memory_order_release);
}

void write_y()
{
  y.store(true, std::memory_order_release);
}

void read_x_then_y()
{
  while(!x.load(std::memory_order_acquire));
  if (y.load(std::memory_order_acquire))
    ++z;
}

void read_y_then_x()
{
  while(!y.load(std::memory_order_acquire));
  if (x.load(std::memory_order_acquire))
    ++z;
}

std::atomic<int> data[3];
std::atomic<int> sync(0);

void thread_1()
{
  data[0].store(1, std::memory_order_relaxed); 
  data[1].store(-1, std::memory_order_relaxed); 
  data[2].store(1000, std::memory_order_relaxed); 
  sync.store(1, std::memory_order_release);
}

void thread_2()
{
  int expected = 1;
  while (!sync.compare_exchange_strong(expected, 2, std::memory_order_acq_rel))
  expected = 1;
}

void thread_3()
{
  while (sync.load(std::memory_order_acquire) < 2);
  assert(data[0].load(std::memory_order_relaxed) == 1);
  assert(data[1].load(std::memory_order_relaxed) == -1);
  assert(data[2].load(std::memory_order_relaxed) == 1000);
}

void test_mem_order()
{
  std::thread a(thread_1);
  std::thread b(thread_2);
  std::thread c(thread_3);

  a.join();
  b.join();
  c.join();
}
