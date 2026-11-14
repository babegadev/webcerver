CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic

.PHONY: default debug release clean

default: debug

debug: CFLAGS += -g -fsanitize=address,undefined
debug: cat

release: CFLAGS += -O2
release: cat

cat: cat.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf cat *o cat.dSYM