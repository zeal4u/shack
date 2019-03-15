#include "safe_set.h"

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

int main(int argc, char* argv[]) 
{
  if (argc != 3) {
     printf("Need specify class name and repeat times\n");
     return 0; 
  }
  
  int times = atoi(argv[2]);
  if (strcmp("-Fine", argv[1]) == 0) {
    safe::FineSafeSet fss;
    std::thread th1(process_add, &fss, times);
    std::thread th2(process_add, &fss, times);
    std::thread th3(process_add, &fss, times);
    std::thread th4(process_remove, &fss, times);
    std::thread th5(process_remove, &fss, times);
    
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
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
    //std::thread th2(process_add, &css, times);
    //std::thread th3(process_add, &css, times);
    //std::thread th4(process_remove, &css, times);
    std::thread th5(process_remove, &css, times);
    
    th1.join();
    //th2.join();
    //th3.join();
    //th4.join();
    th5.join();
    //css.print();
  }
  return 0;
}
