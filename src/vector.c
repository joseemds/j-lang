#include "vector.h"
#include <stdio.h>

Vector *vector_init(size_t element_size) {
  Vector *v = (Vector *)malloc(sizeof(Vector));
  if (!v)
    return NULL;

  v->element_size = element_size;
  v->size = 0;
  v->capacity = 1;
  v->data = malloc(v->capacity * element_size);
  if (!v->data) {
    free(v);
    return NULL;
  }
  return v;
}

void vector_push(Vector *v, void *element) {
  if (v->size >= v->capacity) {
    v->capacity += 1;
    v->data = realloc(v->data, v->capacity * v->element_size);
  }
  void *target = (char *)v->data + v->size * v->element_size;
  memcpy(target, element, v->element_size);
  v->size++;
}

void *vector_get(Vector *v, size_t index) {
  if (index >= v->size) {
    fprintf(stderr, "Index %zu out of bounds (size = %zu)\n", index, v->size);
    exit(EXIT_FAILURE);
  }
  return (char *)v->data + index * v->element_size;
}

void vector_free(Vector *v) {
  if (v) {
    free(v->data);
    free(v);
  }
}

int vector_pop(Vector *v, void *out_element) {
  if (v->size == 0)
    return 0;

  v->size--;
  void *source = (char *)v->data + v->size * v->element_size;

  if (out_element != NULL)
    memcpy(out_element, source, v->element_size);

  return 1;
}

Vector *vector_from_array(void *source_array, size_t count,
                          size_t element_size) {
  Vector *v = vector_init(element_size);
  if (!v)
    return NULL;

  if (v->capacity < count) {
    v->capacity = count;
    v->data = realloc(v->data, v->capacity * v->element_size);
    if (!v->data) {
      free(v);
      return NULL;
    }
  }

  memcpy(v->data, source_array, count * element_size);
  v->size = count;

  return v;
}
