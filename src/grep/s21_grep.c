#include "s21_grep.h"

int main(int argc, char *argv[]) {
  typeflags flags;
  memset(&flags, 0, sizeof(flags));
  char *pattern[const_buff] = {0};
  char *txt[const_buff] = {0};
  int start_read = 0;
  int countertxt = 0;
  if (argc <= 2)
    fprintf(stderr,
            ">>> usage: grep [-ivclnhso] [-e pattern] [-f file] [file ...]");
  if (parsingFlags(argc, argv, &flags, pattern) == 0) {
    if (parsingFiles(txt, argv, argc, &countertxt) == 0) {
      if (efChecker(pattern, txt, &start_read) == 0) {
        int realcountertxt = countertxt - start_read;
        for (int i = start_read; txt[i]; i++)
          s21_grep(&flags, txt[i], pattern, realcountertxt);
      }
    }
  }
  freeArray(pattern, txt);
  return 0;
}

int parsingFlags(int argc, char *argv[], typeflags *flags, char **pattern) {
  int i = 0, fail = 0, counterpattern = 0;
  while (-1 != (i = getopt_long(argc, argv, "e:ivclnhsf:o", longFlag, NULL))) {
    switch (i) {
      case 'e':
        flags->e = 1;
        pattern[counterpattern] = malloc(sizeof(char) * (strlen(optarg) + 1));
        if (pattern[counterpattern] == NULL) break;
        strcpy(pattern[counterpattern], optarg);
        memset(optarg, '\0', 1);
        counterpattern++;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        getPattern(optarg, pattern, &counterpattern);
        memset(optarg, '\0', 1);
        break;
      case 'o':
        flags->o = 1;
        break;
      case '?':
        fprintf(
            stderr,
            ">>> usage: grep [-ivclnhso] [-e pattern] [-f file] [file ...]");
    }
  }
  return fail;
}

int parsingFiles(char **txt, char **argv, int argc, int *countertxt) {
  int j = 1, fail = 0, p = 0;
  while (j < argc) {
    if (argv[j][0] != '-' && argv[j][0] != '\0') {
      txt[p] = malloc(sizeof(char) * (strlen(argv[j]) + 1));
      if (txt[p] == NULL) {
        fprintf(stderr, ">>> malloc allocated \n");
        fail = 2;
        break;
      }
      strcpy(txt[p], argv[j]);
      p++;
    }
    j++;
  }
  if (txt[0] == NULL) {
    fprintf(stderr, ">>> input file");
    fail = 1;
  }
  *countertxt = p;
  return fail;
}

int getPattern(char *argv, char **pattern, int *counterpattern) {
  int fail = 0;
  FILE *file = fopen(argv, "r");
  if (file == NULL) {
    fprintf(stderr, ">>> grep: %s: file doesn't exist or permision denied \n",
            argv);
    fail = 1;
  } else {
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    while ((read = getline(&line, &len, file)) > 0) {
      pattern[*counterpattern] = malloc(sizeof(char) * (read + 1));
      if (pattern[*counterpattern] == NULL) {
        fprintf(stderr, ">>> malloc allocated \n");
        fail = 2;
        break;
      }
      strcpy(pattern[*counterpattern], line);
      pattern[*counterpattern][strcspn(pattern[*counterpattern], "\r\n")] =
          '\0';
      if (!strlen(pattern[*counterpattern])) {
        pattern[*counterpattern][0] = '.';
      }
      (*counterpattern)++;
    }
    fclose(file);
    free(line);
  }
  return fail;
}

int efChecker(char **pattern, char **txt, int *start_read) {
  int fail = 0;
  if (*pattern == NULL) {
    *start_read = 1;
    *pattern = malloc(sizeof(char) * (strlen(*txt) + 1));
    if (*pattern == NULL) {
      fail = 1;
    } else {
      strcpy(*pattern, *txt);
    }
  }
  return fail;
}

void s21_grep(typeflags *flags, char *txtname, char **pattern,
              int realcountertxt) {
  FILE *file = fopen(txtname, "r");
  if (file == NULL) {
    if (flags->s == 0)
      fprintf(stderr, ">>> [%s] No such file or direcotory!\n", txtname);
  } else {
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    char *finded[const_buff] = {NULL};
    int number = 0;
    int m = 0;
    while ((read = getline(&line, &len, file)) > 0) {
      number++;
      if (finder(line, pattern, finded, flags) != 0) {
        m++;
        if (flags->c == 0 && flags->l == 0) {
          if (flags->h == 0 && realcountertxt > 1) printf("%s:", txtname);
          if (flags->n == 1) printf("%d:", number);
          if (flags->o == 1 && flags->v == 0) {
            for (int j = 0; finded[j]; j++) printf("%s\n", finded[j]);
          } else {
            if (line[strlen(line) - 1] == '\n') {
              printf("%s", line);
            } else {
              printf("%s\n", line);
            }
          }
        }
      }
      for (int j = 0; finded[j]; j++) free(finded[j]);
    }
    if (flags->c == 1) {
      if (flags->h == 0 && realcountertxt > 1) printf("%s:", txtname);
      if (flags->l == 1)
        printf("%d\n", m > 0);
      else
        printf("%d\n", m);
    }
    if (flags->l == 1 && m) printf("%s\n", txtname);
    free(line);
    fclose(file);
  }
}

int finder(char *line, char **pattern, char **finded, typeflags *flags) {
  int index = 0, result;
  for (int i = 0; pattern[i]; i++) {
    regex_t regex;
    if (flags->i == 1) {
      regcomp(&regex, pattern[i], REG_ICASE);
    } else {
      regcomp(&regex, pattern[i], REG_EXTENDED);
    }
    regmatch_t find;
    size_t reg_offset = 0;
    size_t linel = strlen(line);
    for (int j; (j = regexec(&regex, line + reg_offset, 1, &find, 0)) == 0;
         index++) {
      int l = find.rm_eo - find.rm_so;
      finded[index] = malloc(l + 1);
      if (finded[index] == NULL) break;
      memcpy(finded[index], line + find.rm_so + reg_offset, l);
      finded[index][l] = '\0';
      reg_offset += find.rm_eo;
      if (reg_offset > linel) break;
    }
    finded[index] = NULL;
    regfree(&regex);
  }
  if (flags->v == 1)
    result = !index;
  else
    result = index;
  return result;
}

void freeArray(char **pattern, char **txt) {
  for (int i = 0; pattern[i]; i++) free(pattern[i]);
  for (int i = 0; txt[i]; i++) free(txt[i]);
}
