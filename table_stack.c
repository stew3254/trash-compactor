#include <string.h>
#include "table_stack.h"


// Get the element from the first place in the stack if it exists at all
// Returns -1 if it's not there at all
int table_stack_get(
    table_stack *s,
    char *id,
    symbol** sym_p
) {
  int val;
  // Loop through all places in the list and see if it's in any of the maps
  for (list_node *n = s->data->head->next; n != s->data->tail; n = n->next) {
    if ((val = map_get(s->data->head->next->e, (void **) id, sizeof(char), strlen(id), (void **) sym_p)) != -1) {
      break;
    }
  }

  // Return the result
  return val;
}

// Print the table_stack
void table_stack_print(const table_stack *s) {
  if (s->data->len == 0)
    printf("{}");

  for (list_node *n = s->data->head->next; n != s->data->tail; n = n->next) {
    hashmap *scope = n->e;
    map_print(scope, "%s", "%p");
    if (n->next != s->data->tail)
      printf(",\n");
  }
}
