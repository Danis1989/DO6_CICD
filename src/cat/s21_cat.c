#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int fail = 0, i = 1;
    opt_struct opt = {
        .b = 0,
        .e = 0,
        .n = 0,
        .s = 0,
        .t = 0,
        .v = 0,
    };
    if (checkInputFilename(&i, argc, argv) == 1) {
      fprintf(stderr, "\n\033[95minput filename");
    } else {
      if (parsingOpt(argc, argv, &opt) == 1) fail = 1;
      if (opt.b == 1) opt.n = 0;
      if (fail == 1) {
        fprintf(
            stderr,
            "functioning options only: [-b, --number-nonblank], [-e, -Ev],\n "
            "[-n, --number], [-s, --squeeze-blank], [-t, -Tv], [-v]");
      } else {
        s21_cat(argc, argv, i, opt);
      }
    }
  } else {
    fprintf(stderr, "\n\033[95muse pattern \"cat [OPTION] [FILE]...\"");
  }
  return 0;
}

void s21_cat(int argc, char *argv[], int i, opt_struct opt) {
  i = 1;
  for (; i < argc; ++i) {
    if (argv[i][0] == '-') {
    } else {
      FILE *openfile;
      if (fopen(argv[i], "r") == NULL) {
        perror("\n\033[95mcat");
      } else {
        openfile = fopen(argv[i], "r");
        catting(openfile, opt);
        fclose(openfile);
      }
    }
  }
}

int parsingOpt(int argc, char *argv[], opt_struct *opt) {
  int opindex = 0, opchar = 0, fail = 0;
  static struct option opts[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
  };
  // int q;
  // for (int i = 0; i < argc; i++)
  //  if (argv[i][0] != '-') q = i;
  while (-1 !=
         (opchar = getopt_long(argc, argv, "+beEnstTv", opts, &opindex))) {
    switch (opchar) {
      case 'b':
        opt->b = 1;
        break;
      case 'e':
        opt->e = 1;
        opt->v = 1;
        break;
      case 'E':
        opt->e = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 't':
        opt->t = 1;
        opt->v = 1;
        break;
      case 'T':
        opt->t = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case '?':
        fail = 1;
        break;
    }
  }
  return fail;
}

int checkInputFilename(int *i, int argc, char *argv[]) {
  int fail = 0;
  for (; *i < argc; ++*i) {
    if (strspn(argv[*i], "-") == 0) break;
  }
  if (*i == argc) {
    fail = 1;
  }
  return fail;
}

void catting(FILE *openfile, opt_struct opt) {
  int begin = 1, empty_counter = 1, counter = 1;
  char last_1 = '\n', last_2 = ' ', z = 0;
  char current_char[1024] = {'\0'};
  while ((*current_char = fgetc(openfile)) != EOF) {
    z = 0;
    if ((opt.s == 1) &&
        (*current_char == '\n' && last_1 == '\n' && last_2 == '\n'))
      continue;
    if (opt.n == 1) {
      if (begin == 1) printf("%6d\t", (counter)++);
      if (*current_char == '\n')
        begin = 1;
      else
        begin = 0;
    }
    if (opt.b == 1) {
      if (last_1 == '\n' && *current_char != '\n')
        printf("%6d\t", (empty_counter)++);
    }
    if (opt.e == 1 && *current_char == '\n') printf("$");
    if (opt.v == 1) {
      if ((*current_char > 126 || *current_char < 32) && *current_char != 9 &&
          *current_char != 10) {
        char c = *current_char;
        int p = c < 32 ? c + 64 : c - 64;
        printf("^%c", p);
        z = 1;
        memset(current_char, '\0', 1024);
      }
    }
    last_2 = last_1;
    last_1 = *current_char;
    if ((opt.t == 1) && *current_char == '\t') {
      printf("^I");
      memset(current_char, '\0', 1024);
      continue;
    }
    if (z == 0) {
      if (*current_char == '\0')
        fputc(*current_char, stdout);
      else
        fputs(current_char, stdout);
    }
    memset(current_char, '\0', 1024);
  }
  memset(current_char, '\0', 1024);
}
