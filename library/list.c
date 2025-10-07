#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct list {
  size_t max_size;
  size_t curr_size;
  void **data;
  free_func_t free;
} list_t;

list_t *list_init(size_t initial_capacity, free_func_t freer) {
  list_t *new = malloc(sizeof(list_t));
  assert(new != NULL);
  new->data = malloc(sizeof(void *) * initial_capacity);
  assert(new->data != NULL);
  new->max_size = initial_capacity;
  new->curr_size = 0;
  new->free = freer;
  return new;
}

void list_free(list_t *list) {
  if (list->free != NULL) {
    for (size_t i = 0; i < list->curr_size; i++) {
      list->free(list->data[i]);
    }
  }
  free(list->data);
  free(list);
}

size_t list_size(list_t *list) { return list->curr_size; }

void *list_get(list_t *list, size_t index) {
  assert(0 <= index && index < list->curr_size);
  return list->data[index];
}

void list_add(list_t *list, void *value) {
  if (list->curr_size == list->max_size) {
    list->max_size *= 2;
    list->data = realloc(list->data, sizeof(void *) * list->max_size);
    assert(list->data != NULL);
  }
  assert(value != NULL);

  list->data[list->curr_size] = value;
  list->curr_size++;
}

void *list_remove(list_t *list, size_t index) {
  assert(index >= 0 && index < list->curr_size);
  void *old_value = list->data[index];

  for (size_t i = index; i < list->curr_size - 1; i++) {
    list->data[i] = list->data[i + 1];
  }
  list->curr_size--;

  return old_value;
}

void remove_entire_list(list_t *list) {
  for (size_t i = 0; i < list_size(list); i++) {
    list_remove(list, i);
  }
}
