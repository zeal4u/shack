#include <unistd.h>
#include <stdlib.h>

int main() {
  execl("/bin/echo", "Hello world!");
  system("echo Hello world!");
  return 0;
} 
