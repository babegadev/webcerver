#include <stdio.h>
#include <string.h>

typedef struct {
  const char* ptr;
  size_t len;
} str_slice;

static size_t scan_until(const char* buf, size_t len, size_t* i, char c) {
  size_t start = *i;
  while (*i < len && buf[*i] != c) {
    (*i)++;
  }
  return *i - start;
}

static int split_request(const char* request, size_t request_len,
                         str_slice* method, str_slice* path,
                         str_slice* version) {
  size_t i = 0;
  method->ptr = request;
  method->len = scan_until(request, request_len, &i, ' ');
  if (method->len == 0 || i == request_len) return -1;
  i++;
  path->ptr = &request[i];
  path->len = scan_until(request, request_len, &i, ' ');
  if (path->len == 0 || i == request_len) return -1;
  i++;
  version->ptr = &request[i];
  version->len = scan_until(request, request_len, &i, ' ');
  if (version->len == 0 || request_len != i) return -1;

  return 0;
}

int main(void) {
  const char* line = "GET /index.html HTTP/1.1";

  str_slice method, path, version;

  if (split_request(line, strlen(line), &method, &path, &version) != 0) {
    printf("bad request line\n");
    return 1;
  }

  printf("method:  %.*s (len %zu)\n", (int)method.len, method.ptr, method.len);
  printf("path:    %.*s (len %zu)\n", (int)path.len, path.ptr, path.len);
  printf("version: %.*s (len %zu)\n", (int)version.len, version.ptr,
         version.len);
  return 0;
}