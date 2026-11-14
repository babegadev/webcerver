#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

static int copy(int inputFd, const char* name) {
  ssize_t numRead;
  char buf[BUF_SIZE];

  while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
    if (write(STDOUT_FILENO, buf, numRead) != numRead) {
      perror("write");
      return -1;
    }
  }

  if (numRead == -1) {
    perror(name);
    return -1;
  }

  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
  int fd;

  if (argc < 2) {
    return copy(STDIN_FILENO, "stdin") == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
  }

  for (int i = 1; i < argc; i++) {
    fd = open(argv[i], O_RDONLY);
    if (fd == -1) {
      perror(argv[i]);
      return EXIT_FAILURE;
    }

    if (copy(fd, argv[i]) == -1) {
      close(fd);
      return EXIT_FAILURE;
    }

    if (close(fd) == -1) {
      perror(argv[i]);
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}