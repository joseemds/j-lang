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
  int length = snprintf(NULL, 0, "%d", i);

  char *buffer = malloc(length + 1);
  if (!buffer) {
    return NULL;
  }

  sprintf(buffer, "%d", i);

  return buffer;
}

char *string_concat(char *a, char *b) {
  size_t len_a = strlen(a);
  size_t len_b = strlen(b);

  char *new_str = malloc(len_a + len_b + 1);

  if (new_str == NULL) {
    fprintf(stderr, "Fatal: failed to allocate memory for string_concat.\n");
    exit(1);
  }
  strcpy(new_str, a);
  strcat(new_str, b);

  return new_str;
}
