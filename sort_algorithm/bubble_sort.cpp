/**
 * Unstable sort
 * Space complexity O(0)
 * Time complexity O(n^2)
 */

#include "sort.h"

#include <utility>

bool bubble_sort(int* num, int len)
{
  if (num == nullptr || len <= 0) return false;
  for (int i = 0; i < len - 1; ++i) {
    for (int j = 0; j < len - i - 1; ++j) {
      if (num[j] > num[j+1]) std::swap(num[j], num[j+1]);
    }
  }
  return true;
}
