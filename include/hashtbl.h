#pragma once

#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 128

typedef struct Entry {
  char *key;
  Symbol *value;
  struct Entry *next;
} Entry;

typedef struct HashTable {
  Entry *buckets[SIZE];
} HashTable;

// Função de hash simples (djb2)
unsigned long hash(const char *str);

void hashtbl_init(HashTable *tbl);

void hashtbl_put(HashTable *tbl, const char *key, Symbol *value);

Symbol *hashtbl_get(HashTable *tbl, const char *key);

void hashtbl_free(HashTable *tbl);
