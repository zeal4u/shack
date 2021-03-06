/**
 * Unstable sort
 * Space complexity O(0)
 * Time complexity O(nlogn)
 * Baddest situation O(n^2)
 */
#include "sort.h"

#include <stdio.h>
#include <vector>
#include <utility>

// pivot does not swap in early procedure
bool quick_sort(int* num, int len)
{
  if (num == nullptr || len < 0) return false;
  else if (len <= 1) return true;

  int j = 0, k = len - 1;
  for (int i = 0; i < k; ++i) {
    if (num[i] < num[k])
      std::swap(num[j++], num[i]);
  }
  std::swap(num[j], num[k]);
  return quick_sort(num, j) && quick_sort(num + j + 1, len - j - 1);
}

// pivot participates in swap
bool faster_sort(int *num, int len)
{
  if (num == nullptr || len < 0) return false;
  else if (len <= 1) return true;
  int i = 0, j = len -1, pivot = num[i];
  while (i < j) {
    while (i < j && num[j] >= pivot) j--;
    num[i] = num[j];
    while (i < j && num[i] <= pivot) i++;
    num[j] = num[i];
  }
  num[i] = pivot;
  return faster_sort(num, i) && faster_sort(num + i + 1, len - i - 1);
}
