#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "vec.h"

// Initialize a new empty vec
vec *vec_with_cap(
    int (*cmp) (const void *a, const void *b),
    void *(*copy) (const void *e),
    void (*del) (void *e),
    unsigned int capacity
) {
  vec *v = (vec *) malloc(sizeof(vec));
  v->cap = capacity;
  v->len = 0;
  v->data = calloc(v->cap + 1, sizeof(ptrdiff_t));
  v->cmp = cmp;
  v->copy = copy;
  v->del = del;
  return v;
}

// Delete an existing vec
void vec_del(vec *v) {
  for (int i = 0; i < v->len; ++i) {
    // Remove the element if it's on the heap
    v->del(*(v->data+i));
  }
  free(v);
}

// Grow the array to the specified size and zero out the extra spaces
void *vec_grow_array(void *a, size_t size, unsigned int *capacity, unsigned int len) {
  // Grow capacity
  while (*capacity <= len) {
    if (*capacity == 0)
      *capacity = 1;
    else
      *capacity *= 2;
  }
  a = realloc(a, (size * *capacity)+1);
  // Make sure to set the last value to 0
  memset(a+len, 0, (*capacity-len+1)*size);
  // Assign the new array
  return a;
}

// Grow the vec by the capacity specified. Returns -1 if it failed
int vec_grow(vec *v, unsigned int capacity) {
  v->cap += capacity;
  void **p;
  if ((p = realloc(v->data, (sizeof(ptrdiff_t) * v->cap)+1)) == NULL) {
    v->cap -= capacity;
    return -1;
  }
  // Make sure to set the last value to 0
  v->data[v->cap] = NULL;
  // Assign the new array
  v->data = p;
  return 0;
}

// Shrink the vec by the capacity specified. Returns -1 if it failed
int vec_shrink(vec *v, unsigned int capacity) {
  // Set capacity to no smaller than the length
  v->cap = (v->cap - capacity < v->len) ? v->len : v->cap - capacity;

  void **p;
  if ((p = realloc(v->data, (sizeof(ptrdiff_t) * v->cap)+1)) == NULL) {
    v->cap += capacity;
    return -1;
  }

  // Make sure to set the last value to 0
  v->data[v->cap] = NULL;
  // Assign the new array
  v->data = p;
  return 0;
}

// Set the capacity. Will not be smaller than the current length
int vec_set_cap(vec *v, unsigned int capacity) {
  unsigned int prev_cap = v->cap;
  // Set capacity to no smaller than the length
  v->cap = (capacity < v->len) ? v->len : capacity;

  void **p;
  if ((p = realloc(v->data, (sizeof(ptrdiff_t) * v->cap)+1)) == NULL) {
    v->cap = prev_cap;
    return -1;
  }
//  memset()

  // Make sure to set the last value to 0
  v->data[v->cap] = NULL;
  // Assign the new array
  v->data = p;
  return 0;
}

// Get a node at a specific index position
void *vec_get(const vec *v, const unsigned int index) {
  if (index >= v->len) {
    return NULL;
  }

  return v->data[index];
}

// Find the first element in the vec based on comparison function for more advanced checks
// Returns -1 if nothing was found
int vec_find_with(const vec *v, const void *e, int (*cmp)(const void *a, const void *b)) {
  for (int i = 0; i < v->len; ++i) {
    if (cmp(e, v->data[i]) == 0)
      return i;
  }
  return -1;
}

// Insert element after index into vector
// Returns -1 if failed
int vec_insert(vec *v, const unsigned int index, void *e) {
  if (v->len == v->cap) {
    if (vec_grow(v, v->cap*2) == -1) {
      return -1;
    }
  }
  // Increase vector length and add element
  v->data[v->len++] = e;
  return 0;
}

// Pops the element in the vector
void *vec_pop(vec *v, unsigned int index) {
//  v->
}

// Reverses the vec in place
vec *vec_rev(vec *v) {
  unsigned int i = 0;
  unsigned int j = v->len-1;
  while (i < j) {
    swap_ptr(v->data+(i++), v->data+(j--));
  }
  return v;
}

// Concatenates 2 vecs with copy function and returns a new vec
// Must be of the same type if you want this to work correctly
vec *vec_concat(vec *v1, vec* v2) {
  // If either vec is empty, no need to concatenate. Just copy the other one
  if (v1->len == 0)
    return vec_copy(v2);
  if (v2->len == 0)
    return vec_copy(v1);

  // Make a new capacity that is a power of 2 larger than the length of the 2 vecs added together
  int cap = 1;
  while(cap < v1->len + v2->len)
    cap <<= 1;

  // Make a new vec
  vec *new_v = vec_with_cap(v1->cmp, v1->copy, v1->del, cap);

  // Copy the items over from v1
  for (unsigned int i = 0; i < v1->len; ++i) {
    new_v->data[i] = v1->copy(v1->data[i]);
  }

  // Copy the items over from v2
  for (unsigned int i = 0; i < v2->len; ++i) {
    new_v->data[i+v1->len] = v2->copy(v2->data[i]);
  }

  // Capture the new length
  new_v->len = v1->len + v2->len;

  return new_v;
}

// Print the contents of a vec for debugging
void vec_print(const vec *v, const char *format) {
  for (unsigned int i = 0; i < v->len; ++i) {
    printf(format, v->data[i]);
  }
}

// Print the contents of a vec between nodes for debugging
void vec_print_between(const vec *v, unsigned int i, unsigned int j, const char *format) {
  // Does not flip nodes if they are backwards unlike other function
  for (unsigned int idx = i; idx < j; ++idx)
    printf(format, v->data[i]);
}
