#ifndef vec_h
#define vec_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "simple_functions.h"

typedef struct Vec {
    void **data;
    unsigned int cap;
    unsigned int len;
    int (*cmp) (const void *a, const void *b);
    void *(*copy) (const void *e);
    void (*del) (void *e);
} vec;

/* Create a vec with capacity */
vec *vec_with_cap(
    int (*cmp) (const void *a, const void *b),
    void *(*copy) (const void *e),
    void (*del) (void *e),
    unsigned int capacity
);

// Create a vec of correct data type size
static inline vec *vec_new(
    int (*cmp) (const void *a, const void *b),
    void *(*copy) (const void *e),
    void (*del) (void *e)
) {
  return vec_with_cap(cmp, copy, del, 32);
}

/* Destroy a vec and all elements inside */
// Not exactly meant to be called directly, main logic behind destroying the vec
void vec_del(vec *v);

/* Capacity Management */
// Grow the array by the capacity specified. Returns -1 if it failed
void *vec_grow_array(void *a, size_t size, unsigned int *capacity, unsigned int len);
// Grow the vec by the capacity specified. Returns -1 if it failed
int vec_grow(vec *v, unsigned int capacity);
// Shrink the vec by the capacity specified or to the current length. Returns -1 if it failed
int vec_shrink(vec *v, unsigned int capacity);
// Set the capacity. Will not be smaller than the current length
int vec_set_cap(vec *v, unsigned int capacity);

/* Finding nodes / positions */
// Get a node at a specific index position
void *vec_get(const vec *v, unsigned int index);

/* Simple searching functions */
// Find the first element in the vec
// Returns -1 if nothing was found
int vec_find(const vec *v, const void *e);

/* Insert into vec after specific node pointer */
// Inserts item after index into vector
int vec_insert(vec *v, unsigned int index, void *e);
// Add something to the end of the vec
static inline int vec_push_back(vec *v, void *e) {
  return vec_insert(v, v->len, e);
}

/* Pop from the vec and retrieve the element inside */
// Pops the element in the vector
void *vec_pop(vec *v, unsigned int index);
// Remove something to the end of the vec
static inline void *vec_pop_back(vec *v) {
  return v->data[--(v->len)];
}

/* Utility functions */
// Reverses the vec in place
vec *vec_rev(vec *v);

// Does a deep copy of elements into a new vec
static inline vec *vec_copy(const vec *v) {
  vec *new_v = vec_with_cap(v->cmp, v->copy, v->del, v->cap);
  for (unsigned int i = 0; i < v->len; ++i) {
    vec_push_back(new_v, v->copy(v->data[i]));
  }
  return new_v;
}

// Concatenates 2 vecs and returns a new vec
// Must be of the same type if you want this to work correctly
vec *vec_concat(vec *v1, vec* v2);

/* Print functions. Prints format per node */
// Prints the contents of the whole vec
void vec_print(const vec *v, const char *format);
// Prints the contents of the whole vec with a newline at the end
static inline void vec_println(const vec *v, const char *format) {
  vec_print(v, format);
  printf("\n");
}
// Prints the contents of the whole vec between 2 indices
void vec_print_between(const vec *v, unsigned int i, unsigned int j, const char *format);
// Prints the contents of the whole vec between 2 indices with a newline at the end
static inline void vec_println_between_indices(
    const vec *v,
    unsigned int i,
    unsigned int j,
    const char *format
) {
  vec_print_between(v, i, j, format);
  printf("\n");
}

#endif