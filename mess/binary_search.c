#include <stdio.h>

int binary_search(int num[], int len, int target)
{
  if (len <= 0)
    return -1;
  int l = 0, h = len - 1, mid = (l + h) >> 1;
  while (l < h) {
    if (target > num[mid]) {
      l = mid + 1;
    } else {
      h = mid;
    }
    mid = (l + h) >> 1;
  }
  if (num[h] == target)
    return h;
  return -1;
}

int main(int argc, char* argv[])
{
  int num[] = {0, 1, 2, 3, 4};
  printf("Search %d, index %d\n", 0, binary_search(num, 5, 0));  
  printf("Search %d, index %d\n", 4, binary_search(num, 5, 4));  
  printf("Search %d, index %d\n", 5, binary_search(num, 5, 5));  
  int num1[] = {4};
  printf("Search %d, index %d\n", 5, binary_search(num1, 1, 5));  
  printf("Search %d, index %d\n", 4, binary_search(num1, 1, 4));  
  int num2[] = {};
  printf("Search %d, index %d\n", 5, binary_search(num1, 0, 5));  
  return 0;
}
