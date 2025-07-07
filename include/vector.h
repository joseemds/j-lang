#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct Vector{
  void *data;         // Ponteiro genérico para os dados
  size_t element_size; // Tamanho de cada elemento (em bytes)
  size_t size;         // Número de elementos atuais
  size_t capacity;     // Capacidade total
} Vector;

Vector *vector_init(size_t element_size);
void vector_push(Vector *v, void *element);
void *vector_get(Vector *v, size_t index);
void vector_free(Vector *v);
int vector_pop(Vector *v, void *out_element);