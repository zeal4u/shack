#include <stdio.h>
#include <stdlib.h>

typedef struct Byte{
  char content[2];
}Byte;

int main(int argc, char* argv[])
{
  if (argc == 2) {
    int len = atoi(argv[1]);
    Byte *a = new Byte[len];
    for (int i = 0; i < len; ++i) {
      printf("0x%llx ",&a[i]);
    }
    printf("\n");
    delete[] a;
  }
  return 0;
}
