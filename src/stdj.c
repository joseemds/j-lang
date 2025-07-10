#include "stdj.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float int_to_float(int v) { return (float)v; }

int str_to_int(char *value) {
  char *endptr;
  errno = 0;
  long result = strtol(value, &endptr, 10);

  if (errno != 0 || *endptr != '\0') {
    fprintf(stderr, "Invalid integer input: '%s'\n", value);
    exit(1);
  }

  return (int)result;
}

char *input() {
  size_t size = 64;
  char *buffer = malloc(size);
  if (!buffer) {
    perror("malloc");
    exit(1);
  }

  size_t len = 0;
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) {
    if (len + 1 >= size) {
      size *= 2;
      buffer = realloc(buffer, size);
      if (!buffer) {
        perror("realloc");
        exit(1);
      }
    }
    buffer[len++] = ch;
  }
  buffer[len] = '\0';
  return buffer;
}

char *int_to_string(int i) {
  static char buffer[12];
  sprintf(buffer, "%d", i);
  return buffer;
}
