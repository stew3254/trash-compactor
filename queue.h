#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

typedef struct Queue {
    list *data;
    void *(*copy) (const void *e);
    void (*del) (void *e);
} queue;

// Create a enw queue
static inline queue *queue_new(void *(*copy) (const void *e), void (*del) (void *e)) {
  queue *q = malloc(sizeof(queue));
  // We don't actually need to do any comparisons
  q->data = list_new(simple_cmp, q->copy = copy, q->del = del);
  return q;
}

// Delete a queue
static inline void queue_del(queue *q) {
  list_del(q->data);
  free(q);
}

// Get the length of the queue
static inline unsigned int queue_len(queue *q) {
  return q->data->len;
}

// Get the top element off of the queue
static inline void *queue_get(queue *q) {
  return q->data->head->next->e;
}

// Push an element onto the queue
static inline void queue_push(queue *q, void *e) {
  list_push_back(q->data, e);
}

// Pop an element off of the queue
static inline void *queue_pop(queue *q) {
  return list_pop_front(q->data);
}

// Copy the queue
static inline queue *queue_copy(queue *q) {
  queue *new_q = malloc(sizeof(queue));
  new_q->data = list_copy(q->data);
  new_q->copy = q->copy;
  new_q->del = q->del;
  return new_q;
}

// Print the queue
static inline void queue_print(queue *q, char *format) {
  list_print(q->data, format);
}

// Print the queue with a newline
static inline void queue_println(queue *q, char *format) {
  list_println(q->data, format);
}

#endif
