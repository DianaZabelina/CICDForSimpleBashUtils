#include "s21_cat.h"

int main(int argc, char *argv[]) {
  (void)argv;
  if (argc == 1) {
    if (!cat_no_args(STDIN_FILENO, argv[0])) return 1;
  } else {
    if (!cat_args(argc, argv)) return 1;
  }
  return 0;
}

int cat_no_args(int fd, char *name) {
  char buffer[4096];
  int bytes_read;
  if (fd == -1) {
    perror(name);
    return 1;
  }

  bytes_read = read(fd, buffer, 4096);
  while (bytes_read > 0) {
    printf("%.*s", bytes_read, buffer);
    bytes_read = read(fd, buffer, 4096);
  }
  return bytes_read == -1;
}

int arg_str(CatInfo *info, char *argv, char *name) {
  if (strcmp(argv, "number-nonblank") == 0) {
    info->num_not_empty = 1;
    info->num_all = 0;
  } else if (strcmp(argv, "number") == 0) {
    if (info->num_not_empty == 0) info->num_all = 1;
  } else if (strcmp(argv, "squeeze-blank") == 0) {
    info->squeeze = 1;
  } else {
    fprintf(stderr, "%s: illegal option --%s\n", name, argv);
    info->error = 1;
    return 0;
  }
  return 1;
}

int arg_ch(CatInfo *info, char *argv, char *name) {
  for (char *item = argv; *item; item++) {
    switch (*item) {
      case 'b':
        info->num_not_empty = 1;
        info->num_all = 0;
        break;
      case 'e':
        info->show_endl = 1;
        info->show_non_print = 1;
        break;
      case 'E':
        info->show_endl = 1;
        break;
      case 'n':
        if (info->num_not_empty == 0) info->num_all = 1;
        break;
      case 's':
        info->squeeze = 1;
        break;
      case 't':
        info->show_tabs = 1;
        info->show_non_print = 1;
        break;
      case 'T':
        info->show_tabs = 1;
        break;
      default:
        fprintf(stderr,
                "%s: illegal option -- %s\nusage: cat [-benstuv] [file ...]\n",
                name, argv);
        info->error = 1;
        return 0;
    }
  }
  return 1;
}

int cat_parse_arg(CatInfo *info, char *argv, char *name) {
  argv++;
  if (*argv == '-') {
    argv++;
    return arg_str(info, argv, name);
  }
  return arg_ch(info, argv, name);
}

int cat_args(int argc, char *argv[]) {
  CatInfo info = {0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < argc; i++) {
    if (*argv[i] == '-') {
      if (!cat_parse_arg(&info, argv[i], argv[0])) return 0;
    }
  }
  if (!(info.num_not_empty || info.show_endl || info.num_all || info.squeeze ||
        info.show_tabs || info.show_non_print)) {
    for (int i = 1; i < argc; i++) {
      if (*argv[i] != '-') {
        if (!cat_no_args(open(argv[i], O_RDONLY), argv[0])) info.error = 1;
      }
    }
  } else {
    int n_str = 1;
    for (int i = 1; i < argc; i++) {
      if (*argv[i] != '-') {
        if (!cat_perform(&info, argv[i], argv[0], &n_str)) info.error = 1;
      }
    }
  }
  return info.error;
}

void opt_num(CatInfo info, const int *end, int c, int *n_str) {
  if (((info.num_not_empty && c != '\n') || info.num_all) && *end == '\n') {
    printf("%6u\t", (unsigned int)(*n_str));
    *n_str += 1;
  }
}

int opt_show_tabs(CatInfo info, int c) {
  if (info.show_tabs && c == '\t') {
    printf("^");
    c = '\t' + 64;
  }
  return c;
}

int opt_show_non_print(CatInfo info, int c) {
  if (info.show_non_print) {
    if (c > 127 && c < 160) printf("M-^");
    if (c > -1 && c != '\n' && c != '\t') {
      if (c < 32 || c == 127) printf("^");
      if (c < 32) c = c + 64;
      if (c > 126 && c < 160) c = c - 128 + 64;
    }
  }
  return c;
}

void print_symb(CatInfo info, int *end, int c, int *empty, int *n_str) {
  if (!(info.squeeze && *end == '\n' && c == '\n' && *empty)) {
    if (*end == '\n' && c == '\n') {
      *empty = 1;
    } else
      *empty = 0;
    opt_num(info, end, c, n_str);
    if (info.show_endl && c == '\n') printf("$");
    c = opt_show_tabs(info, c);
    c = opt_show_non_print(info, c);
    fputc(c, stdout);
  }
  *end = c;
}

int cat_perform(CatInfo *info, char *filename, char *name, int *n_str) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror(name);
    return 1;
  } else {
    int empty = 0;

    int c = fgetc(file), end = '\n';
    while (c != EOF) {
      print_symb(*info, &end, c, &empty, n_str);
      c = fgetc(file);
    }
    fclose(file);
  }
  return 0;
}