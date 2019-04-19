#include "safe_set.h"
#include "safe_queue.h"
#include "safe_lookup_table.h"

#include <thread>
#include <vector>
#include <string.h>
#include <random>

void process_add(safe::AbstractSafeSet* ass, int times)
{
  std::default_random_engine dre;
  while (times--) {
    ass->add(dre());  
  }
}

void process_remove(safe::AbstractSafeSet* ass, int times)
{
  std::default_random_engine dre;
  while (times--) {
    ass->remove(dre());  
  }
}

void test_safe_set(int argc, char *argv[])
{
  printf("Max concurrency threads: %d\n", std::thread::hardware_concurrency());
  if (argc != 3) {
     printf("Need specify class name and repeat times\n");
		 return ;
  }
  
  int times = atoi(argv[2]);
  if (strcmp("-Fine", argv[1]) == 0) {
    safe::FineSafeSet fss;
    std::thread th[5];
    int i = 0;
    for (; i < 3; i++) {
      th[i] = std::thread(process_add, &fss, times);
    }
    while (i--) {
      th[i].join();
    }
    //std::thread th1(process_add, &fss, times);
    //std::thread th2(process_add, &fss, times);
    //std::thread th3(process_add, &fss, times);
    //std::thread th4(process_remove, &fss, times);
    //std::thread th5(process_remove, &fss, times);
    //
    //th1.join();
    //th2.join();
    //th3.join();
    //th4.join();
    //th5.join();
    //fss.print();
  } else if (strcmp("-Coarse", argv[1]) == 0) {
    safe::FineSafeSet css;
    std::thread th1(process_add, &css, times);
    std::thread th2(process_add, &css, times);
    std::thread th3(process_add, &css, times);
    std::thread th4(process_remove, &css, times);
    std::thread th5(process_remove, &css, times);
    
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    //css.print();
  } else if (strcmp("-Unblocking", argv[1]) == 0) {
    safe::UnblockingSafeSet css;
    std::thread th1(process_add, &css, times);
    std::thread th2(process_add, &css, times);
    std::thread th3(process_add, &css, times);
    std::thread th4(process_remove, &css, times);
    std::thread th5(process_remove, &css, times);
    
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    //css.print();
  }
}

void process_push(safe::SafeQueue<int> *sq, int times)
{
	std::default_random_engine dre;
  while (times--) {
		int i = dre();
		printf("Push %d\n", i);
    sq->push(i);  
  }
}

void process_pop(safe::SafeQueue<int> *sq, int times)
{
	while (times--) {
		auto res = sq->wait_and_pop();
		printf("Pop %d\n", *res);
	}
}

void test_safe_queue(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Need spacify threads number and repeat times!");
		return;
	}
	int nums = atoi(argv[1]);
	int times = atoi(argv[2]);
	std::thread *threads = new std::thread[nums]();
	safe::SafeQueue<int> sq;
	for (int i = 0; i < nums; ++i) {
		if ((i & 1) == 1)
			threads[i] = std::move(std::thread(process_pop, &sq, times));
		else
			threads[i] = std::move(std::thread(process_push, &sq, times));
	}

	for (int i = 0; i < nums; ++i) {
		threads[i].join();
	}
	delete[] threads;
}

void test_safe_lookup_table()
{
	safe::SafeLookupTable<int, int> slt;
	// TODO
}

int main(int argc, char* argv[]) 
{
	test_safe_lookup_table();
	return 0;
}
