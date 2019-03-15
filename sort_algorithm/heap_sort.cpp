/**
 * Unstable sort
 * Space complexity O(0)
 * Time complexity O(nlogn)
 */
#include "sort.h"

#include <utility>
#include <string.h>

bool init_heap(int* num, int len);
bool adjust_heap(int* num, int len, int i);

bool heap_sort(int* num, int len)
{
  if (num == nullptr || len <= 0) return false;
  else if (len <= 1) return true;
  bool ret = true;
  ret &= init_heap(num, len);
  for (int i = len - 1; i > 0; --i) {
    std::swap(num[0], num[i]);
    ret &= adjust_heap(num, i, 0);
  }
  return ret;      
}

bool init_heap(int* num, int len)
{
  if (num == nullptr || len <= 0) return false;
  else if (len <= 1) return true;
  for (int i = (len - 2) >> 1; i >= 0; --i) {
    adjust_heap(num, len, i);
  }
  return true;
}

bool adjust_heap(int* num, int len, int i)
{
  if (num == nullptr || len <= 0) return false;
  else if (len <= 1) return true;
  for (; i < len;) {
    int nex_i = 0;
    if (2 * i + 2 < len && num[2 * i + 1] < num[2 * i + 2])
      nex_i = 2 * i + 2;
    else nex_i = 2 * i + 1;
    if (nex_i < len && num[i] < num[nex_i]) {
      std::swap(num[i], num[nex_i]);
      i = nex_i;
    } else break;
  }
  return true;
}

bool small_k(int* output, int* num, int len, int k)
{
  if (num == nullptr || len <= 0 || len < k) return false;
  memcpy(output, num, k * sizeof(int));
  if (len == k) {
    return true;
  }
  bool ret = true;
  ret &= init_heap(output, k); 
  for (int i = k; i < len; ++i) {
    if (num[i] < output[0]) {
      std::swap(num[i], output[0]);
      ret &= adjust_heap(output, k, 0);
    }
  }
  return ret;
}
