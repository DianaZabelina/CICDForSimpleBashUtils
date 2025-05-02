#include <ctype.h>
#include <fcntl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
  int pattern;
  int ignore_case;
  int invert;
  int count_str;
  int count_files;
  int num_str;
  int hide_filenames;
  int hide_errors;
  int error;
} GrepInfo;

FILE *open_file(char *filename);
void print_file_error(char *name, char *filename);
int regex_compilation(regex_t *regex, char *patt, int ign_case);
void print_string(char *str);
int grep_no_args(FILE *file, int multiple, char *filename, char *input_regex);
int arg_ch(GrepInfo *info, char *argv, char *name, int *step);
char **add_pattern(char **regexes, int *count, char *patt);
void free_array(char **array, int rows);
int grep_args(int argc, char *argv[]);
int check_invert(GrepInfo info, regex_t *regex, char *patt);
void check_c_n_h_flags(GrepInfo info, char *filename, int multiple, int *n_str,
                       char *line, int *c_str);
void check_count_files(int *c_files, int *c_str);
void check_counts(GrepInfo info, char *filename, int multiple, int c_str,
                  int c_files);
int grep_perform(GrepInfo *info, FILE *file, char *filename, char **regexes,
                 int count, int multiple);