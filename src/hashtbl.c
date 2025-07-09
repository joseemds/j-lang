#include "hashtbl.h"

unsigned long hash(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;
  return hash % SIZE;
}

HashTable *hashtbl_init() {
  HashTable *tbl = (HashTable *)malloc(sizeof(HashTable));
  if (!tbl) {
    return NULL;
  }

  for (int i = 0; i < SIZE; i++) {
    tbl->buckets[i] = NULL;
  }

  return tbl;
}

void hashtbl_put(HashTable *tbl, const char *key, Symbol *value) {
  unsigned long index = hash(key);
  Entry *entry = tbl->buckets[index];

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      entry->value = value;
      return;
    }
    entry = entry->next;
  }

  Entry *new_entry = malloc(sizeof(Entry));
  new_entry->key = strdup(key);
  new_entry->value = value;
  new_entry->next = tbl->buckets[index];
  tbl->buckets[index] = new_entry;
}

Symbol *hashtbl_get(HashTable *tbl, const char *key) {
  unsigned long index = hash(key);
  Entry *entry = tbl->buckets[index];

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return entry->value; // sucesso
    }
    entry = entry->next;
  }
  return NULL; // não encontrado
}

void hashtbl_free(HashTable *tbl) {
  for (int i = 0; i < SIZE; i++) {
    Entry *entry = tbl->buckets[i];
    while (entry != NULL) {
      Entry *next = entry->next;
      free(entry->key);
      // free(entry->value);
      free(entry);
      entry = next;
    }
  }

  free(tbl);
}
