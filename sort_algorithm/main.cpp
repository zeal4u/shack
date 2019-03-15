#include <stdio.h>
#include <stdlib.h>

#include "sort.h"

int main(int argc, char* argv[])
{
  //int nums[argc - 1];
  //for (int i = 0; i < argc - 1; ++i)
  //  nums[i] = atoi(argv[i + 1]);
  //if (nums != nullptr && heap_sort(nums, argc - 1))
  //for (int i = 0; i < argc - 1; ++i) {
  //  printf("%d ", nums[i]);
  //}
  //printf("\n");
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
  return 0;
}
