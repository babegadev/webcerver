#include <stdlib.h>

int main(void) {
  char* hello = malloc(5 * sizeof(char));

  for (size_t i = 0; i <= 5; i++) {
    hello[i] = ' ';
  }

  free(hello);
  return 0;
}