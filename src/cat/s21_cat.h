#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int num_not_empty;
  int show_endl;
  int num_all;
  int squeeze;
  int show_tabs;
  int show_non_print;
  int error;
} CatInfo;

int cat_no_args(int fd, char *name);
int cat_parse_arg(CatInfo *info, char *argv, char *name);
int arg_str(CatInfo *info, char *argv, char *name);
int arg_ch(CatInfo *info, char *argv, char *name);
int cat_args(int argc, char *argv[]);
void opt_num(CatInfo info, const int *end, int c, int *n_str);
int opt_show_tabs(CatInfo info, int c);
int opt_show_non_print(CatInfo info, int c);
void print_symb(CatInfo info, int *end, int c, int *empty, int *n_str);
int cat_perform(CatInfo *info, char *filename, char *name, int *n_str);