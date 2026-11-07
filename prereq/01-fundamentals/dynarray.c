#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int* data;
  size_t len;
  size_t cap;
} IntVec;

/* Set up an empty vector with room for initial_cap items (0 is allowed).
   Returns 0 on success, -1 if malloc fails. */
int intvec_init(IntVec* v, size_t initial_cap) {
  v->data = malloc(initial_cap * sizeof(*v->data));
  if (initial_cap != 0 && v->data == NULL) {
    return -1;
  }
  v->cap = initial_cap;
  v->len = 0;
  return 0;
}

/* Free the data and reset len and cap to 0. Safe to call twice. */
void intvec_free(IntVec* v) {
  free(v->data);
  v->data = NULL;
  v->cap = 0;
  v->len = 0;
}

/* Private helper used by push: grow to at least min_cap. Returns 0 or -1. */
static int intvec_grow(IntVec* v, size_t min_cap) {
  size_t new_cap = v->cap == 0 ? min_cap : v->cap * 2;
  if (v->cap > SIZE_MAX / 2 || new_cap > SIZE_MAX / sizeof(*v->data)) return -1;
  int* bigger = realloc(v->data, new_cap * sizeof(*v->data));
  if (bigger == NULL) {
    return -1;
  }
  v->data = bigger;
  v->cap = new_cap;
  return 0;
}

/* Add value at the end, doubling cap first if len == cap.
   Returns 0 on success, -1 if growing fails (v is left unchanged). */
int intvec_push(IntVec* v, int value) {
  if (v->len == v->cap) {
    if (intvec_grow(v, v->cap + 1) != 0) {
      return -1;
    };
  }
  v->data[v->len] = value;
  v->len++;
  return 0;
}

/* Read the item at index into *out. Returns 0, or -1 if index >= len. */
int intvec_get(const IntVec* v, size_t index, int* out) {
  if (index >= v->len) {
    return -1;
  }
  *out = v->data[index];
  return 0;
}

/* Overwrite the item at index. Returns 0, or -1 if index >= len. */
int intvec_set(IntVec* v, size_t index, int value) {
  if (index >= v->len) {
    return -1;
  }
  v->data[index] = value;
  return 0;
}

/* Remove the last item and write it to *out. Returns 0, or -1 if empty. */
int intvec_pop(IntVec* v, int* out) {
  if (v->len == 0) {
    return -1;
  }
  *out = v->data[v->len - 1];
  v->len--;
  return 0;
}

static int is_power_of_two(size_t n) { return n != 0 && (n & (n - 1)) == 0; }

int main(void) {
  IntVec v;
  int x;

  /* 1. Start from zero capacity and push past many doublings. */
  assert(intvec_init(&v, 0) == 0);
  assert(v.len == 0 && v.cap == 0);
  for (int i = 0; i < 1000; i++) {
    assert(intvec_push(&v, i * 10) == 0);
    assert(v.len == (size_t)i + 1);
    assert(v.cap >= v.len);
    assert(is_power_of_two(v.cap));
  }

  /* 2. Every value is where we put it. */
  for (size_t i = 0; i < v.len; i++) {
    assert(intvec_get(&v, i, &x) == 0);
    assert(x == (int)i * 10);
  }

  /* 3. Out-of-range get/set fail and leave things untouched. */
  x = -12345;
  assert(intvec_get(&v, v.len, &x) == -1);
  assert(x == -12345);
  assert(intvec_set(&v, v.len, 7) == -1);
  assert(intvec_get(&v, (size_t)-1, &x) == -1);

  /* 4. set overwrites in place. */
  assert(intvec_set(&v, 500, 42) == 0);
  assert(intvec_get(&v, 500, &x) == 0 && x == 42);
  assert(intvec_set(&v, 500, 5000) == 0);

  /* 5. pop returns items in reverse order, then fails when empty. */
  size_t cap_before = v.cap;
  for (int i = 999; i >= 0; i--) {
    assert(intvec_pop(&v, &x) == 0);
    assert(x == i * 10);
  }
  assert(v.len == 0);
  assert(v.cap == cap_before); /* popping doesn't shrink */
  assert(intvec_pop(&v, &x) == -1);

  /* 6. The vector is reusable after being emptied. */
  assert(intvec_push(&v, 99) == 0);
  assert(intvec_get(&v, 0, &x) == 0 && x == 99);

  /* 7. Free is safe twice. */
  intvec_free(&v);
  assert(v.data == NULL && v.len == 0 && v.cap == 0);
  intvec_free(&v);

  /* 8. Nonzero starting capacity: no growth until full, then doubles. */
  assert(intvec_init(&v, 3) == 0);
  for (int i = 0; i < 3; i++) assert(intvec_push(&v, i) == 0);
  assert(v.cap == 3);
  assert(intvec_push(&v, 3) == 0);
  assert(v.cap == 6);
  intvec_free(&v);

  printf("all tests passed\n");
  return 0;
}