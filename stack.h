#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

typedef struct Stack {
    list *data;
    void *(*copy) (const void *e);
    void (*del) (void *e);
} stack;

// Create a new stack
static inline stack *stack_new(void *(*copy) (const void *e), void (*del) (void *e)) {
  stack *s = malloc(sizeof(stack));
  // We don't actually need to do any comparisons
  s->data = list_new(simple_cmp, s->copy = copy, s->del = del);
  return s;
}

// Delete a stack
static inline void stack_del(stack *s) {
  list_del(s->data);
  free(s);
}

// Get the length of the stack
static inline unsigned int stack_len(stack *s) {
  return s->data->len;
}

// Get the top element off of the stack
static inline void *stack_get(stack *s) {
  return s->data->head->next->e;
}

// Push an element onto the stack
static inline void stack_push(stack *s, void *e) {
  list_push_front(s->data, e);
}

// Pop an element off of the stack
static inline void *stack_pop(stack *s) {
  return list_pop_front(s->data);
}

// Copy the stack
static inline stack *stack_copy(const stack *s) {
  stack *new_s = malloc(sizeof(stack));
  new_s->data = list_copy(s->data);
  new_s->copy = s->copy;
  new_s->del = s->del;
  return new_s;
}

// Print the stack
static inline void stack_print(stack *s, char *format) {
  list_print(s->data, format);
}

// Print the stack with a newline
static inline void stack_println(stack *s, char *format) {
  list_println(s->data, format);
}

#endif
