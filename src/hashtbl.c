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

void hashmap_put(HashTable *map, const char *key, int value) {
  unsigned long index = hash(key);
  Entry *entry = map->buckets[index];

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
  new_entry->next = map->buckets[index];
  map->buckets[index] = new_entry;
}

int hashmap_get(HashTable *map, const char *key, int *out_value) {
  unsigned long index = hash(key);
  Entry *entry = map->buckets[index];

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      *out_value = entry->value;
      return 1; // sucesso
    }
    entry = entry->next;
  }
  return 0; // não encontrado
}

void hashmap_free(HashTable *map) {
  for (int i = 0; i < SIZE; i++) {
    Entry *entry = map->buckets[i];
    while (entry != NULL) {
      Entry *next = entry->next;
      free(entry->key);
      free(entry->value);
      free(entry);
      entry = next;
    }
  }

  free(tbl);
}
