#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "sort.h"

void test_small_k(int argc, char *argv[])
{
  int k = atoi(argv[1]);
  int nums[argc - 2];
  for (int i = 0; i < argc - 2; ++i) {
    nums[i] = atoi(argv[i + 2]);
  }
  int output[k];
  if (nums != nullptr && small_k(output, nums, argc - 2, k)) {
    for (int i = 0; i < k; ++i)
      printf("%d ", output[i]);
    printf("\n");
  }
}

void test_tradition_sort(int argc, char *argv[], bool sort(int*, int)) {
  if (argc < 2) {
    printf("Need more parameters!\n");
    return;
  }
  int nums[argc - 1];
  for (int i = 0; i < argc - 1; ++i)
    nums[i] = atoi(argv[i + 1]);
  if (nums != nullptr && sort(nums, argc - 1))
  for (int i = 0; i < argc - 1; ++i) {
    printf("%d ", nums[i]);
  }
  printf("\n");
}

void test_parallel_quick_sort()
{
  std::list<int> test_list = {1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1, 1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1, 1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1, 1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1, 1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1, 1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1, 1, 35, -1, 3, 4, 90, 321, -987, 1,1, 35, -1, 3, 4, 90, 321, -987, 1};
  test_list = parallel_quick_sort(test_list);
  for_each(test_list.begin(), test_list.end(), [](int i){ std::cout<<i<<" ";});
	std::cout<<std::endl;
}
int main(int argc, char* argv[])
{
  //test_tradition_sort(argc, argv, faster_sort);
  test_parallel_quick_sort();
}
