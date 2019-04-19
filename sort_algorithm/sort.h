#ifndef _SORT_H_
#define _SORT_H_

#include "parallel_sort.h"

extern bool quick_sort(int* num, int len);
extern bool faster_sort(int* num, int len);
extern bool bubble_sort(int* num, int len);
extern bool merge_sort(int* num, int len);
extern bool insert_sort(int* num, int len);
extern bool heap_sort(int* num, int len);
extern bool small_k(int* output, int* num, int len, int k);
#endif
