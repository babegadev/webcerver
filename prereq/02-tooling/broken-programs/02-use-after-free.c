#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char* name = "Alang";

  printf("Name: %s\n", name);

  free(name);

  printf("Name: %s\n", name);

  return 0;
}