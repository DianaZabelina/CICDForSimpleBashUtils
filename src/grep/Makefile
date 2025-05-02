CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
MAKE=MAKE

s21_grep: s21_grep.o
	$(CC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c

clean:
	rm -rf *.o s21_grep

.PHONY: rebuild
rebuild:
	$(MAKE) clean
	$(MAKE) s21_grep
