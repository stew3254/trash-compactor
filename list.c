#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

// Initialize a new empty list
list *list_new(
    int (*cmp) (const void *a, const void *b),
    void *(*copy) (const void *e),
    void (*del) (void *e)
) {
  list *l = (list *) malloc(sizeof(list));
  // Get 2 different pointers for checking later
  l->head = list_new_node(NULL);
  l->tail = list_new_node(NULL);
  // Remember to initialize length
  l->len = 0;
  l->cmp = cmp;
  l->copy = copy;
  l->del = del;

  // Set head to face tail and vice versa
  l->head->next = l->head->prev = l->tail;
  l->tail->next = l->tail->prev = l->head;
  return l;
}

// Delete an existing list
void list_del(list *l) {
  // Save the start pointer
  list_node *start = l->head;
  list_node *n = l->head;
  list_node *temp_n = n;
  do {
    // Store pointer to next element
    temp_n = n->next;
    // Remove the element if it's on the heap
    if (n->e != NULL)
      l->del(n->e);

    // Delete n
    free(n);
    // Set it to the next pointer
    n = temp_n;
  } while (n != start);
  free(l);
}

list_node *list_get_at(const list *l, const unsigned int index) {
  if (index >= l->len) {
    return NULL;
  }
  list_node *n;

  bool isForward = true;
  if (l->len/2 - index <= 0)
    isForward = false;

  if (isForward) {
    n = l->head->next;
    for (int i = 0; i < index; i++) {
      n = n->next;
    }
  } else {
    n = l->tail->prev;
    for (int i = 0; i < index; i++) {
      n = n->prev;
    }
  }
  return n;
}

// Find position of node in the list
int list_get_at_pos(const list *l, const list_node* item) {
  int pos = 0;
  for (list_node *n = l->head; n != item; n = n->next)
    ++pos;
  return pos;
}

// Find the first element in the list based on comparison function for more advanced checks
// Returns NULL if nothing was found
list_node *list_find_with(const list *l, const void *e, int (*cmp)(const void *a, const void *b)) {
  list_node *n;
  for (n = l->head; n != l->head; n = n->next) {
    if (cmp(e, n->e) == 0)
      return n;
  }
  return NULL;
}

// Insert element after item
list_node *list_insert(list *l, list_node *item, void *e) {
  list_node *new_node = list_new_node(e);

  // Shift the pointer so it points to the element before the fake tail
  if (item == l->tail)
    item = item->prev;

  // Set back pointer to the item being inserted after
  new_node->prev = item;
  // Set next pointer to the item after the one we're inserting after
  new_node->next = item->next;
  // Set the forward pointer of item and back pointer of the item after the new node to the new node
  item->next = new_node->next->prev = new_node;

  // Bump up len
  ++l->len;
  return new_node;
}

// Pop item
// Prone to memory leaking if try to free item twice
void *list_pop(list *l, list_node *item) {
  void *e = item->e;
  item->prev->next = item->next;
  item->next->prev = item->prev;
  free(item);
  --l->len;
  return e;
}

// Reverses the list in place
list *list_rev(list *l) {
  list_node *n = l->head;
  list_node *next;

  do {
    // Move forward in the list
    next = n->next;
    // Flip the pointers
    swap_ptr((void **) &n->next, (void **) &n->prev);
    // Continue to move forward
    n = next;
  } while (n != l->head);
  // Since the head and tail got reversed, change their order in the list
  swap_ptr((void **) &l->head, (void **) &l->tail);
  return l;
}

// Does a deep copy of elements into a new list. Allows you to specify how to deep copy
list *list_copy_with(const list *l, void *(copy)(const void *e)) {
  list *new_l = list_new(l->cmp, l->copy, l->del);
  for (list_node *n = l->head->next; n != l->tail; n = n->next) {
    list_push_back(new_l, copy(n->e));
  }
  return new_l;
}

// Concatenates 2 lists with copy function and returns a new list
list *list_concat_with(list *l1, list* l2, void *(copy)(const void *e)) {
  // If either list is empty, no need to concatenate. Just copy the other one
  if (l1->len == 0)
    return list_copy_with(l2, copy);
  if (l2->len == 0)
    return list_copy_with(l1, copy);

  // Make a new list without the head and tail
  list *new_l = (list*) malloc(sizeof(list));
  // Copy the 2 lists since these will be consumed
  list *new_l1 = list_copy_with(l1, copy);
  list *new_l2 = list_copy_with(l2, copy);

  /* Stitch all of the nodes together */
  // Set the head
  new_l->head = new_l1->head;
  // Set the tail, but also the previous pointer on the head ot the right tail
  new_l->head->prev = new_l->tail = new_l2->tail;
  // Set the tail's next to the right head
  new_l->tail->next = new_l->head;
  // Fix the last element in list 1 to point to the first element of list 2
  list_node *temp = new_l1->tail->prev;
  new_l1->tail->prev->next = new_l2->head->next;
  // Do the opposite
  new_l2->head->next->prev = temp;


  // Update the new length
  new_l->len = l1->len + l2->len;

  // Free the unused nodes in the now consumed lists
  free(new_l1->tail);
  free(new_l2->head);
  // Free the consumed lists
  free(new_l1);
  free(new_l2);

  return new_l;
}

// Concatenates 2 lists but consumes them both
list *list_concat_consume_with(
    list *l1,
    list* l2,
    void *(*copy) (const void *e),
    void (*del) (void *e)
) {
  // Make sure they aren't the same list
  if (l1 == l2) {
    // Make a copy of it that we can consume later
    l2 = list_copy_with(l1, copy);
  }

  // If either list is empty, no need to concatenate. Just give it the other one
  if (l1->len == 0) {
    list_del(l1);
    return l2;
  }
  if (l2->len == 0) {
    list_del(l2);
    return l1;
  }

  // Capture the old head and tail since we will need to free them later
  list_node *old_head = l2->head;
  list_node *old_tail = l1->tail;

  /* Stitch all of the nodes together */
  // Set the tail, but also the previous pointer on the head ot the right tail
  l1->head->prev = l1->tail = l2->tail;
  // Set the tail's next to the right head
  l1->tail->next = l1->head;
  // Fix the last element in list 1 to point to the first element of list 2
  list_node *temp = l1->tail->prev;
  old_tail->prev->next = l2->head->next;
  // Do the opposite
  old_head->next->prev = temp;

  // Update the new length
  l1->len += l2->len;

  // Free the unused nodes in the now consumed lists
  free(old_head);
  free(old_tail);
  // Free the consumed list
  free(l2);

  return l1;
}

// Print the contents of a list for debugging
void list_print(const list *l, const char *format) {
  for (list_node *n = l->head->next; n->next != l->head; n = n->next) {
    printf(format, n->e);
  }
}

// Print the contents of a list between nodes for debugging
void list_print_between(const list *l, const list_node *i, const list_node *j, const char *format) {
  // Does not flip nodes if they are backwards unlike other function
  list_node *n = i;
  while(n != j) {
    // Print the format per node
    printf(format, n->e);
    n = n->next;
  }
}

// Print the contents of a list between indices for debugging
void list_print_between_indices(const list *l, unsigned int i, unsigned int j, const char *format) {
  // Flips signs if they are backwards
  if (i > j) {
    list_print_between_indices(l, j, i, format);
  }

  // Get the location of the node
  list_node *n = list_get_at(l, i);
  while(i != j) {
    // Print the format per node
    printf(format, n->e);
    n = n->next;
    ++i;
  }
}
