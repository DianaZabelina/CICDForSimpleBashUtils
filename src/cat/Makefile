CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
MAKE=MAKE

s21_cat: s21_cat.o
	$(CC) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c

clean:
	rm -rf *.o s21_cat

.PHONY: rebuild
rebuild:
	$(MAKE) clean
	$(MAKE) s21_cat
