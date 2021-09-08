#ifndef list_h
#define list_h

#include <stdbool.h>
#include "simple_functions.h"

typedef struct ListNode {
    void *e;
    struct ListNode *next;
    struct ListNode *prev;
} list_node;

typedef struct List {
    list_node *head;
    list_node *tail;
    unsigned int len;
    int (*cmp) (const void *a, const void *b);
    void *(*copy) (const void *e);
    void (*del) (void *e);
} list;

// Create a list
list *list_new(
    int (*cmp) (const void *a, const void *b),
    void *(*copy) (const void *e),
    void (*del) (void *e)
);

// Destroy a list and all elements inside
void list_del(list *l);

// Create a new node to add to the list. Not meant to call this directly
static inline list_node *list_new_node(void *e) {
  list_node *node_ptr = (list_node*) malloc(sizeof(list_node));
  node_ptr->prev = node_ptr->next = node_ptr;
  node_ptr->e = e;
  return node_ptr;
}

/* Finding nodes / positions */
// Get the front node
static inline list_node *list_get_front(const list *l) {
  return l->head->next;
}
// Get the back node
static inline list_node *list_get_back(const list *l) {
  return l->tail->prev;
}
// Get a node at a specific index position. Best used for inserting and popping,
// but is used in insert_at and pop_at
list_node *list_get_at(const list *l, unsigned int index);
// Find position of node in the list
int list_get_pos(const list *l, const list_node* item);

/* Simple searching functions */
// Find the first element in the list based on comparison function for more advanced checks
// Returns NULL if nothing was found
list_node *list_find_with(const list *l, const void *e, int (*cmp)(const void *a, const void *b));
// Find the first element in the list where simple comparison works
// Returns NULL if nothing was found
static inline list_node *list_find(const list *l, const void *e) {
  return list_find_with(l, e, simple_cmp);
}

/* Insert into list after specific node pointer */
// Most efficient way to tree_insert_from new items
list_node *list_insert(list *l, list_node *item, void *e);
// Insert into list at certain position, not very efficient
static inline list_node *list_insert_at(list *l, unsigned int i, void *e) {
  return list_insert(l, list_get_at(l, i), e);
}
// Add something to the beginning of the list
static inline list_node *list_push_front(list *l, void *e) {
  return list_insert(l, l->head, e);
}
// Add something to the end of the list
static inline list_node *list_push_back(list *l, void *e) {
  return list_insert(l, l->tail, e);
}

/* Pop from the list and retrieve the element inside */
// Most efficient way to pop
void *list_pop(list *l, list_node *item);
// Pop from list at certain position, not very efficient
static inline void *list_pop_at(list *l, unsigned int i) {
  return list_pop(l, list_get_at(l, i));
}
// Remove something to the beginning of the list
static inline void *list_pop_front(list *l) {
  return list_pop(l, l->head->next);
}
// Remove something to the end of the list
static inline void *list_pop_back(list *l) {
  return list_pop(l, l->tail->prev);
}

/* Utility functions */
// Reverses the list in place
list *list_rev(list *l);

// Does a deep copy of elements into a new list. Allows you to specify how to deep copy
list *list_copy_with(const list *l, void *(*copy)(const void *e));
// Does a shallow copy of elements into a new list. Works well for simple types
static inline list *list_copy(const list *l) {
  return list_copy_with(l, return_elem);
}

// Concatenates 2 lists with copy function and returns a new list
list *list_concat_with(list *l1, list* l2, void *(*copy)(const void *e));
// Concatenates 2 lists and returns a new list
static inline list *list_concat(list *l1, list* l2) {
  return list_concat_with(l1, l2, return_elem);
}
// Concatenates 2 lists with copy function if needed but consumes them both
list *list_concat_consume_with(
    list *l1,
    list* l2,
    void *(*copy)(const void *e),
    void (*del)(void *e)
);
// Concatenates 2 lists but consumes them both
static inline list *list_concat_consume(list *l1, list* l2, void (*del)(void *e)) {
  return list_concat_consume_with(l1, l2, return_elem, del);
}

/* Print functions. Prints format per node */
// Prints the contents of the whole list
void list_print(const list *l, const char *format);
// Prints the contents of the whole list with a newline at the end
static inline void list_println(const list *l, const char *format) {
  list_print(l, format);
  printf("\n");
}
// Prints the contents of the whole list between 2 nodes
void list_print_between(const list *l, const list_node *i, const list_node *j, const char *format);
// Prints the contents of the whole list between 2 nodes with a newline at the end
static inline void list_println_between(
    const list *l,
    const list_node *i,
    const list_node *j,
    const char *format
) {
  list_print_between(l, i, j, format);
  printf("\n");
}
// Prints the contents of the whole list between 2 indices
void list_print_between_indices(const list *l, unsigned int i, unsigned int j, const char *format);
// Prints the contents of the whole list between 2 indices with a newline at the end
static inline void list_println_between_indices(
    const list *l,
    unsigned int i,
    unsigned int j,
    const char *format
) {
  list_print_between_indices(l, i, j, format);
  printf("\n");
}

#endif
