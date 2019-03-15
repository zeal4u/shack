/**
 * Unstable sort
 * Space complexity O(0)
 * Time complexity O(n^2)
*/

#include <string.h>

bool insert_sort(int* num, int len)
{
  if (num == nullptr || len <= 0) return false;
  for (int i = 1; i < len; ++i) {
    int temp = num[i];
    for (int j = i - 1; j >= 0; --j) {
      if (num[j] <= temp) {
        if (j != i - 1) {
          memmove(num + j + 2, num + j + 1, sizeof(int) * (i - j - 1));
          num[j+1] = temp;
        } 
        break;
      }
      if (!j) {
        memmove(num + 1, num, sizeof(int) * i);
        num[0] = temp;
      }
    }
  }  
  return true;
}
