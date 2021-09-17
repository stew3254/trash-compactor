#ifndef HASHMAP_H
#define HASHMAP_H

#include "simple_functions.h"
#include "avl.h"
#include "list.h"

typedef struct HashMap {
    avl_tree **buckets;
    unsigned int bucket_size;
    unsigned long (*hash) (const void* k, size_t n);
    void *(*copy) (const void *e);
    void (*del) (void *e);
    unsigned int len;
} hashmap;

typedef struct Entry {
    void *key;
    void *value;
    unsigned int key_size;
    unsigned int key_len;
} hashmap_entry;


/* Utility functions */
// Default hashing function
unsigned long hashpjw(const void *k, size_t n);
// Comparison function for hashmap entries
// Needed in key searching / sorting in the tree
int map_simple_entry_cmp(const void *a, const void *b);
// Simple entry copy
static inline void* map_simple_entry_copy(const void *e) {
  hashmap_entry *new_e = malloc(sizeof(hashmap_entry));
  new_e->key = ((hashmap_entry *) e)->key;
  new_e->key_size = ((hashmap_entry *) e)->key_size;
  new_e->key_len = ((hashmap_entry *) e)->key_len;
  new_e->value = ((hashmap_entry *) e)->value;
  return new_e;
}
// Simple entry removal
static inline void map_simple_entry_remove(void *e) {
  // Free the key and value
  free(((hashmap_entry *) e)->key);
  free(((hashmap_entry *) e)->value);
  free(e);
}
// Preserve value on removal. Used when you get something from the map first
// And then want to use that same pointer later without copying
static inline void map_value_preserve_entry_remove(void *e) {
  // Free the key and value
  free(((hashmap_entry *) e)->key);
  free(e);
}

// Assigning a different hashing function for the hashmap
hashmap *map_with_hash(
    size_t size,
    unsigned long (*hash) (const void* k, size_t n),
    void *(*copy) (const void *e),
    void (*del) (void *e)
);

// Hashmap with the standard hash function, but different size
static inline hashmap *map_with_size(
    size_t size,
    void *(*copy) (const void *e),
    void (*del) (void *e)
) {
  return map_with_hash(size, hashpjw, copy, del);
}

// Create a new hashmap
static inline hashmap *map_new(void *(*copy) (const void *e), void (*del) (void *e)) {
  return map_with_size(211, copy, del);
}

// Delete a hashmap
static inline void map_del(hashmap *m) {
  // Free all of the trees
  for (size_t i = 0; i < m->bucket_size; ++i)
    avl_tree_del(m->buckets[i]);

  //Free the buckets
  free(m->buckets);
  // Free the map itself
  free(m);
}

// Insert into the map
// The key must be a pointer to the thing you actually want to use
void map_insert(
    hashmap *m,
    void **k,
    unsigned int key_size,
    unsigned int key_len,
    void *v
);

// Get key from the map. If the return value is -1 then the value was not found
// The key must be a pointer to the thing you actually want to use
int map_get(hashmap *m, void **k, unsigned int key_size, unsigned int key_len, void **v);

// Remove key from the map
// The key must be a pointer to the thing you actually want to use
void map_remove(hashmap *m, void **k, unsigned int key_size, unsigned int key_len);

// Get pairs in map
static inline list *map_pairs(const hashmap *m) {
  list *l = avl_tree_to_list(m->buckets[0]);
  for (unsigned int i = 1; i < m->bucket_size; ++i)
    l = list_concat_consume(l, avl_tree_to_list(m->buckets[i]), do_not_del);
  // Reset deletion on list so we don't accidentally free the data
  l->del = do_not_del;
  return l;
}
// Get keys in map
list *map_keys(const hashmap *m);
// Get values in map
list *map_values(const hashmap *m);

// Copy a map
hashmap *map_copy(const hashmap *m);

/* Printing utilities */
// Print a hashmap with given function
void map_print_with(const hashmap *m, void (p)(hashmap_entry *e));
// Print a hashmap with simple formats
void map_print(const hashmap *m, char *key_format, char *value_format);
// Print a hashmap but with a newline at the end
static inline void map_println(const hashmap *m, char *key_format, char *value_format) {
  map_print(m, key_format, value_format);
  printf("\n");
}
// Print keys of a hashmap
void map_keys_print(const hashmap *m, char *format);
// Print a hashmap but with a newline at the end
static inline void map_keys_println(const hashmap *m, char *format) {
  map_keys_print(m, format);
  printf("\n");
}
// Print values of a hashmap
void map_values_print(const hashmap *m, char *format);
// Print a hashmap but with a newline at the end
static inline void map_values_println(const hashmap *m, char *format) {
  map_values_print(m, format);
  printf("\n");
}

#endif
