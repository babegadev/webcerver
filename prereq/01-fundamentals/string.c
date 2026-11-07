#include <stdio.h>

size_t my_strlen(const char* s) {
  size_t i = 0;

  while (s[i] != '\0') {
    i++;
  }

  return i;
}

int my_strncmp(const char* a, const char* b, size_t n) {
  unsigned const char* u1 = (unsigned const char*)a;
  unsigned const char* u2 = (unsigned const char*)b;

  for (size_t i = 0; i < n; i++) {
    if (u1[i] == '\0' || u2[i] == '\0' || u1[i] != u2[i]) {
      return u1[i] - u2[i];
    }
  }
  return 0;
}

int main(void) {
  char* a = "hello";
  char* b = "hellooooooo";

  printf("%s: %zu\n", a, my_strlen(a));
  printf("%s: %zu\n", b, my_strlen(b));
  printf("%d\n", my_strncmp(a, b, 7));

  return 1;
}
