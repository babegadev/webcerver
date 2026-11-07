#include <stdio.h>
#include <stdlib.h>

char* readfile_to_heap_buffer(const char* filename) {
  FILE* fp = fopen(filename, "rb");

  if (fp == NULL) {
    return NULL;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    fclose(fp);
    return NULL;
  }

  long end = ftell(fp);
  if (end < 0) {
    fclose(fp);
    return NULL;
  }
  size_t size = (size_t)end;

  if (fseek(fp, 0, SEEK_SET) != 0) {
    fclose(fp);
    return NULL;
  }

  char* buffer;
  if ((buffer = malloc(size + 1)) == NULL) {
    fclose(fp);
    return NULL;
  }

  if (fread(buffer, 1, size, fp) != size) {
    fclose(fp);
    free(buffer);
    return NULL;
  }

  buffer[size] = '\0';
  fclose(fp);

  return buffer;
}

int main(void) {
  char* result = readfile_to_heap_buffer("hello.txt");
  if (result == NULL) {
    return EXIT_FAILURE;
  }
  printf("%s\n", result);
  free(result);
  return 0;
}