#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 128

typedef struct Entry {
  char *key;
  int value;
  struct Entry *next;
} Entry;

typedef struct HashMap {
  Entry *buckets[HASHMAP_SIZE];
} HashMap;

// Função de hash simples (djb2)
unsigned long hash(const char *str);

void hashmap_init(HashMap *map);

void hashmap_put(HashMap *map, const char *key, int value);

int hashmap_get(HashMap *map, const char *key, int *out_value);

void hashmap_free(HashMap *map);