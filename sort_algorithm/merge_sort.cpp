/**
 * Unstable sort
 * Space complexity O(n)
 * Time complexity O(nlogn)
 */
#include "sort.h"

#include <stdlib.h>
#include <string.h>

bool merge_sort(int* num, int len)
{
  if (num == nullptr || len < 0) return false;
  else if (len <= 1) return true;
  int mid = len >> 1;
  if (merge_sort(num, mid) && merge_sort(num + mid, len - mid)) {
    int i = 0, j = mid, k = 0;
    int* temp = new int[len];
    while (i < mid && j < len) {
      if (num[i] < num[j]) {
        temp[k] = num[i];
        i++;
      } else {
        temp[k] = num[j];
        j++;
      }   
      k++; 
    }
    while (i < mid) temp[k] = num[i], k++, i++;
    while (j < len) temp[k] = num[j], k++, j++;
    memcpy(num, temp, sizeof(int) * len);
    delete [] temp;
    return true;
  }
  return false;
}
