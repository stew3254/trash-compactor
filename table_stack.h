#ifndef STACKED_TABLES_H
#define STACKED_TABLES_H

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "list.h"
#include "hashmap.h"

//Define the TableStack type
typedef stack table_stack;

typedef struct Symbol {
    int type;
    union {
        int ival;
        float rval;
        int opval;
        char *sval;
    } attribute;
} symbol;

static inline symbol *symbol_new(const int type) {
  symbol *new_s = (symbol *) malloc(sizeof(symbol));
  new_s->type = type;
  return new_s;
}

static inline symbol *symbol_with(const int type, void *e) {
  symbol *new_s = (symbol *) malloc(sizeof(symbol));
  new_s->type = type;
  new_s->attribute.sval = e;
  return new_s;
}

// TODO fix this when you can tell it's a string for proper string copy
static inline void *symbol_copy(const void *e) {
  symbol *s = (symbol*) e;
  // Create the new symbol
  symbol *new_s = symbol_new(s->type);
  new_s->attribute.sval = s->attribute.sval;
  return new_s;
}

// TODO fix this for when you can tell it's a string for proper deletion
static inline void symbol_del(void *e) {
  // free the struct
  free(e);
}

// Create a new table_stack
static inline table_stack *table_stack_new() {
  table_stack *t = (table_stack*) stack_new(symbol_copy, symbol_del);
  return t;
}
// Delete a table_stack
static inline void table_stack_del(table_stack *s) {
  list_del(s->data);
  free(s);
}

// Get the length of the table_stack
static inline unsigned int table_stack_len(table_stack *s) {
  return s->data->len;
}

// Insert into the local-most scope of the stack
static inline void table_stack_insert(
    table_stack *s,
    char *id,
    symbol *sym
) {
  return map_insert((hashmap *) s->data->head->next->e, id, sizeof(char), strlen(id), sym);
}

// Get the element from the first place in the stack if it exists at all
// Returns -1 if it's not there at all
int table_stack_get(
    table_stack *s,
    char *id,
    symbol** sym_p
);

// Get the top element off of the table_stack if it exists
static inline int table_stack_get_local(
    table_stack *s,
    char *id,
    symbol** sym_p
) {
  return map_get(s->data->head->next->e, id, sizeof(char), strlen(id), (void **) sym_p);
}

// Add a new empty scope onto the stack
static inline void table_stack_add(table_stack *s) {
  list_push_front(s->data, map_new(symbol_copy, symbol_del));
}

// Push a scope onto the stack
static inline void table_stack_push(table_stack *s, hashmap *scope) {
  list_push_front(s->data, scope);
}

// Pop a scope from the stack
static inline hashmap *table_stack_pop(table_stack *s) {
  return list_pop_front(s->data);
}

// Copy the table_stack
static inline table_stack *table_stack_copy(const table_stack *s) {
  return stack_copy(s);
}

// Print the table_stack
void table_stack_print(const table_stack *s);

// Print the table_stack with a newline
static inline void table_stack_println(const table_stack *s) {
  table_stack_print(s);
  printf("\n");
}

#endif
