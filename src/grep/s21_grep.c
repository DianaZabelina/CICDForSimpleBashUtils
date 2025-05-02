#include "s21_grep.h"

int main(int argc, char *argv[]) {
  (void)argv;
  if (argc == 1) {
    fprintf(stderr,
            "usage: grep [-cilnv] [-e pattern] "
            "[pattern] [file ...]\n");
    return 1;
  } else {
    if (!grep_args(argc, argv)) return 1;
  }
  return 0;
}

FILE *open_file(char *filename) {
  FILE *file = fopen(filename, "r");
  return file;
}

void print_file_error(char *name, char *filename) {
  fprintf(stderr, "%s: %s: No such file or directory\n", name, filename);
}

int regex_compilation(regex_t *regex, char *pattern, int ign_case) {
  int res;
  if (ign_case)
    res = regcomp(regex, pattern, REG_ICASE);
  else
    res = regcomp(regex, pattern, 0);
  if (res) {
    fprintf(stderr, "regex compilation error\n");
    return 1;
  }
  return 0;
}

void print_string(char *str) {
  printf("%s", str);
  if (strchr(str, '\n') == 0) printf("\n");
}

int grep_no_args(FILE *file, int multiple, char *filename, char *input_regex) {
  regex_t regex;
  int reg = regex_compilation(&regex, input_regex, 0);
  if (reg) return 1;
  char line[MAX_LINE_LENGTH];
  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    reg = regexec(&regex, line, 0, NULL, 0);
    if (reg == 0) {
      if (multiple > 3) printf("%s:", filename);
      print_string(line);
    }
  }
  regfree(&regex);
  return 0;
}

int arg_ch(GrepInfo *info, char *argv, char *name, int *step) {
  for (char *item = argv; *item; item++) {
    switch (*item) {
      case 'e':
        info->pattern = 1;
        *step += 1;
        return 2;
      case 'i':
        info->ignore_case = 1;
        break;
      case 'v':
        info->invert = 1;
        break;
      case 'c':
        info->count_str = 1;
        info->num_str = 0;
        break;
      case 'l':
        info->count_files = 1;
        info->num_str = 0;
        break;
      case 'n':
        if (info->count_str == 0 || info->count_files == 0) info->num_str = 1;
        break;
      case 'h':
        if (info->count_files == 0) info->hide_filenames = 1;
        break;
      case 's':
        info->hide_errors = 1;
        break;
      default:
        fprintf(stderr,
                "%s: invalid option -- %s\nusage: grep [-cilnv] [-e pattern] "
                "[pattern] [file ...]\n",
                name, argv);
        info->error = 1;
        return 0;
    }
  }
  return 1;
}

char **add_pattern(char **regexes, int *count, char *patt) {
  char **temp = realloc(regexes, (*count + 1) * sizeof(char *));
  if (temp == NULL) {
    free(regexes);
    fprintf(stderr, "unable to allocate memory\n");
    return NULL;
  }
  regexes = temp;
  regexes[*count] = malloc((strlen(patt) + 1) * sizeof(char));
  if (regexes[*count] == NULL) {
    free(regexes);
    fprintf(stderr, "unable to allocate memory\n");
    return NULL;
  }
  strcpy(regexes[*count], patt);
  (*count)++;

  return regexes;
}

void free_array(char **array, int rows) {
  if (array != NULL) {
    for (int i = 0; i < rows; i++) free(array[i]);
    free(array);
  }
}

int grep_args(int argc, char *argv[]) {
  GrepInfo info = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int step = 1, count = 0;
  char **regexes = NULL;
  for (int i = 0; i < argc; i++) {
    if (*argv[i] == '-') {
      argv[i]++;
      int check = arg_ch(&info, argv[i], argv[0], &step);
      if (check == 2) regexes = add_pattern(regexes, &count, argv[i + 1]);
      step++;
    }
  }
  if (count == 0) {
    regexes = add_pattern(regexes, &count, argv[step]);
    step += 1;
  }
  if (!(info.pattern || info.ignore_case || info.invert || info.count_str ||
        info.count_files || info.num_str || info.hide_filenames ||
        info.hide_errors)) {
    for (int i = 2; i < argc; i++) {
      if (*argv[i] != '-') {
        FILE *file = open_file(argv[i]);
        if (file != NULL) {
          if (!grep_no_args(file, argc, argv[i], argv[1])) info.error = 1;
          fclose(file);
        } else {
          print_file_error(argv[0], argv[i]);
          info.error = 1;
        }
      }
    }
  } else {
    for (int i = step; i < argc; i++) {
      if (*argv[i] != '-') {
        FILE *file = open_file(argv[i]);
        if (file != NULL) {
          if (!grep_perform(&info, file, argv[i], regexes, count,
                            (argc - step)))
            info.error = 1;
          fclose(file);
        } else {
          if (!info.hide_errors) print_file_error(argv[0], argv[i]);
          info.error = 1;
        }
      }
    }
  }
  free_array(regexes, count);
  return info.error;
}

int check_invert(GrepInfo info, regex_t *regex, char *patt) {
  int res;
  if (info.ignore_case)
    res = regex_compilation(regex, patt, 1);
  else
    res = regex_compilation(regex, patt, 0);
  return res;
}

void check_c_n_h_flags(GrepInfo info, char *filename, int multiple, int *n_str,
                       char *line, int *c_str) {
  if (!info.count_str) {
    if (!info.hide_filenames && multiple > 1) printf("%s:", filename);
    if (info.num_str) printf("%d:", *n_str);
    print_string(line);
  } else
    *c_str += 1;
}

void check_count_files(int *c_files, int *c_str) {
  if (!*c_files) {
    *c_str = 1;
    *c_files = 1;
  }
}

void check_counts(GrepInfo info, char *filename, int multiple, int c_str,
                  int c_files) {
  if (info.count_str) {
    if (!info.hide_filenames && multiple > 1)
      printf("%s:%d\n", filename, c_str);
    else
      printf("%d\n", c_str);
  }
  if (info.count_files && c_files) printf("%s\n", filename);
}

int grep_perform(GrepInfo *info, FILE *file, char *filename, char **regexes,
                 int count, int multiple) {
  regex_t regex;
  char line[MAX_LINE_LENGTH];
  int c_str = 0;
  int c_files = 0;
  int n_str = 0;
  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    int printed = 0;
    int inverted_matches = 0;
    if (info->num_str) n_str += 1;
    for (int i = 0; i < count; i++) {
      int reg = check_invert(*info, &regex, regexes[i]);
      if (reg) break;
      reg = regexec(&regex, line, 0, NULL, 0);
      if (!info->invert && reg == 0) {
        if (!info->count_files) {
          if (!printed) {
            check_c_n_h_flags(*info, filename, multiple, &n_str, line, &c_str);
            printed = 1;
          }
        } else
          check_count_files(&c_str, &c_files);

      } else if (info->invert && reg == REG_NOMATCH) {
        inverted_matches += 1;
        if (inverted_matches == count) {
          if (!info->count_files) {
            check_c_n_h_flags(*info, filename, multiple, &n_str, line, &c_str);
          } else
            check_count_files(&c_str, &c_files);
        }
      }
      regfree(&regex);
    }
  }
  check_counts(*info, filename, multiple, c_str, c_files);
  return 0;
}
