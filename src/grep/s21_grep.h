#ifndef S21_CAT_GREP_GREP_S21_GREP_H_
#define S21_CAT_GREP_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define const_buff 8196

typedef struct flags {
  int e; // Шаблон | печать line
  int i; // Игнорирует различия регистра. | печать line | 189
  int v; // Инвертирует смысл поиска соответствий. | печать line |
  int c; // Выводит только количество совпадающих строк. | печать m
  int l; // Выводит только совпадающие файлы. | печать txtname
  int n; // Предваряет каждую строку вывода номером строки из файла ввода. |
         // печать number
  int h; // Выводит совпадающие строки, не предваряя их именами файлов. | печать
         // line
  int s; // Подавляет сообщения об ошибках о несуществующих или нечитаемых
         // файлах. | 141
  int f; // Получает регулярные выражения из файла. | печать line
  int o; // Печатает только совпадающие (непустые) части совпавшей строки. |
         // печать finded в line
} typeflags;

struct option longFlag[] = {
    {"e", 0, 0, 'e'}, {"i", 0, 0, 'i'}, {"v", 0, 0, 'v'}, {"c", 0, 0, 'c'},
    {"l", 0, 0, 'l'}, {"n", 0, 0, 'n'}, {"h", 0, 0, 'h'}, {"s", 0, 0, 's'},
    {"f", 0, 0, 'f'}, {"o", 0, 0, 'o'},
};

int parsingFlags(int argc, char *argv[], typeflags *flags, char **pattern);
int getPattern(char *filename, char **pattern, int *p);
int parsingFiles(char **txt, char **argv, int argc, int *countertxt);
int efChecker(char **pattern, char **txt,
              int *start_read); // добавление pattern в отсутсвие флагов -e -f
void s21_grep(typeflags *flags, char *txtname, char **pattern,
              int realcountertxt);
void freeArray(char **pattern, char **txt);
int finder(char *line, char **pattern, char **finded,
           typeflags *flags); // от найденного index и включенного флага -v
                              // зависит печать line

#endif // S21_CAT_GREP_GREP_S21_GREP_H_
