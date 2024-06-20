#ifndef S21_CAT_GREP_GREP_S21_CAT_H_
#define S21_CAT_GREP_GREP_S21_CAT_H_
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct opti {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} opt_struct;

int parsingOpt(int argc, char *argv[], opt_struct *opt);
int checkInputFilename(int *i, int argc, char *argv[]);
void s21_cat(int argc, char *argv[], int i, opt_struct opt);
void catting(FILE *filename, opt_struct opt);

#endif // S21_CAT_GREP_GREP_S21_CAT_H_