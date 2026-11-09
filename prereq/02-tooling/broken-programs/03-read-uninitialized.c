#include <stdio.h>

int main(void) {
  int x;  // uninitialized (contains garbage);

  printf("x: %d", x);

  return 0;
}