#include "stdio.h"
#include "avl.h"
#include "vec.h"
#include "hashmap.h"

//Default hashing function
unsigned long hashpjw(const void *k, size_t n) {
  unsigned long high;
  unsigned long  h = 0;

  for (size_t i = 0; i < n; ++i) {
    // Casting to a char pointer, so I can only index a single byte at once
    h = (unsigned long) ((h << 4) + ((char *) *((char *) k) + i));
    if ((high = h & 0xF0000000) > 0)
      h ^= high >> 24;
    h &= ~high;
  }
  return h;
}

// Comparison function for hashmap entries
// Needed in key searching / sorting in the tree
int map_simple_entry_cmp(const void *a, const void *b) {
  // Convert to a byte array and compare byte by byte
  char *ak = (char *) &((hashmap_entry *) a)->key;
  char *bk = (char *) &((hashmap_entry *) b)->key;
  size_t a_size = (size_t) ((hashmap_entry *) a)->key_size;
  size_t b_size = (size_t) ((hashmap_entry *) b)->key_size;
  for (size_t i = 0; i < min(a_size, b_size); ++i) {
    if (ak[i] > bk[i])
      return 1;
    else if (ak[i] < bk[i])
      return -1;
  }

  // Make sure one is not longer than the other
  if (a_size > b_size)
    return 1;
  else if (a_size < b_size)
    return -1;

  // They truly are the same, byte by byte and by length
  return 0;
}

hashmap *map_with_hash(
    size_t size,
    unsigned long (*hash) (const void* k, size_t n),
    void *(*copy) (const void *e),
    void (*del) (void *e)
) {
  hashmap *m = (hashmap *) malloc(sizeof(hashmap));
  m->hash = hash;
  m->copy = copy;
  m->del = del;
  m->bucket_size = size;
  m->len = 0;
  m->buckets = malloc(sizeof(avl_tree)*(size+1));
  // Initialize all of the trees
  for (size_t i = 0; i < size; ++i) {
    m->buckets[i] = avl_tree_new(map_simple_entry_cmp, copy, del);
  }
  // Set last bucket to NULL just in case
  m->buckets[size] = NULL;
  return m;
}

// Insert into the map
// The key must be a pointer to the thing you actually want to use
void map_insert(hashmap *m, void **k, unsigned int key_size, unsigned int key_len, void *v) {
  unsigned int index = hashpjw(k, key_size*key_len) % m->bucket_size;
  avl_tree *bucket = m->buckets[index];
  // Get length of bucket
  unsigned int len = bucket->len;

  // Wipe high bytes in key when size is less than 8
  // This is fine if 8 - key_size comes out to be 0
  ptrdiff_t key = *k;
  long shift = 8 - key_size;
  if (shift > 0)
    key = (key << shift*8) >> shift*8;

  // Initialize the entry
  hashmap_entry *e = (void *) malloc(sizeof(hashmap_entry));
  e->key = key;
  e->key_size = key_size;
  e->key_len = key_len;
  e->value = v;

  // Insert into the bucket
  avl_tree_insert(bucket, e);

  // Check to see if the bucket size changed
  if (len < bucket->len)
    ++m->len;
}

// Get key from the map. If value_size is -1 then the value was not found
int map_get(hashmap *m, void **k, unsigned int key_size, unsigned int key_len, void **v) {
  unsigned int index = hashpjw(k, key_size) % m->bucket_size;
  avl_tree *bucket = m->buckets[index];

  // Wipe high bytes in key when size is less than 8
  // This is fine if 8 - key_size comes out to be 0
  ptrdiff_t key = *k;
  long shift = 8 - key_size;
  if (shift > 0)
    key = (key << shift*8) >> shift*8;

  // Make entry to search with
  hashmap_entry entry = {
      .key = key,
      .key_size = key_size,
      .key_len = key_len,
  };

  // Get the result from our tree
  search_result r = avl_tree_get(bucket, &entry);

  // Set the value if we found it
  if (r.found) {
    *v = ((hashmap_entry *) r.e)->value;
    return 0;
  }
  return -1;
}

// Remove key from the map
void map_remove(hashmap *m, void **k, unsigned int key_size, unsigned int key_len) {
  unsigned int index = hashpjw(k, key_size) % m->bucket_size;
  avl_tree *bucket = m->buckets[index];
  // Get bucket length
  unsigned int len = bucket->len;

  // Make entry to search with
  hashmap_entry entry = {
      .key = *k,
      .key_size = key_size,
  };

  avl_tree_remove(bucket, &entry);
  // Check to see if bucket length changed
  if (len > bucket->len)
    --m->len;
}

// Get keys in map
list *map_keys(const hashmap *m) {
  // Set stack del as delete because we never actually want to call free on these elements
  // They could still be in the map and that would be bad
  list *l = list_new(map_simple_entry_cmp, return_elem, do_not_del);
  list *pairs = map_pairs(m);
  for (list_node *n = pairs->head->next; n != pairs->tail; n = n->next)
    list_push_back(l, ((hashmap_entry *)n->e)->key);
  list_del(pairs);
  return l;
}

// Get values in map
list *map_values(const hashmap *m) {
  list *l = list_new(map_simple_entry_cmp, return_elem, do_not_del);
  list *pairs = map_pairs(m);
  for (list_node *n = pairs->head->next; n != pairs->tail; n = n->next)
    list_push_back(l, ((hashmap_entry *)n->e)->value);
  return l;
}

// Copy a map
hashmap *map_copy(const hashmap *m) {
  hashmap *new_m = malloc(sizeof(hashmap));
  new_m->hash = m->hash;
  new_m->copy = m->copy;
  new_m->del = m->del;
  new_m->bucket_size = m->bucket_size;
  new_m->buckets = malloc(sizeof(avl_tree *) * (m->bucket_size + 1));

  // Copy the trees
  for (unsigned int i = 0; i < m->bucket_size; ++i) {
    new_m->buckets[i] = avl_tree_copy(m->buckets[i]);
  }

  // Set the last one null just in case
  new_m->buckets[m->bucket_size] = NULL;
  new_m->len = m->len;

  return new_m;
}

// Print a hashmap (Not exactly the most efficient but only to be used for debugging)
void map_print(const hashmap *m, char *key_format, char *value_format) {
  list *l = avl_tree_to_list(m->buckets[0]);
  for (unsigned int i = 1; i < m->bucket_size; ++i)
    l = list_concat_consume(l, avl_tree_to_list(m->buckets[i]), return_elem);
  if (m->len > 0)
      printf("{\n");
  else
      printf("{");
  for (list_node *n = l->head->next; n != l->tail; n = n->next) {
    // 2 comes from ": " 2 comes from ",\n" and 2 come from the indentation
    unsigned int len = strlen(key_format) + strlen(value_format) + 6;
    char format[len + 1];
    char *pos = format;
    // Make sure the string is null terminating
    format[len] = '\0';

    // Copy the format into the string bit by bit. This sucks
    strcpy(pos, "  ");
    pos += 2;
    strcpy(pos, key_format);
    pos += strlen(key_format);
    strcpy(pos, ": ");
    pos += 2;
    strcpy(pos, value_format);
    pos += strlen(value_format);
    strcpy(pos, ",\n");

    // Print out the entry
    printf(format, ((hashmap_entry *)n->e)->key, ((hashmap_entry *)n->e)->value);
  }
  printf("}");
  // Reset deletion on list so we don't accidentally free the data
  l->del = do_not_del;
  list_del(l);
}

// Print keys of a hashmap
void map_keys_print(const hashmap *m, char *format) {
  list *l = map_pairs(m);

  // Build the new format string
  char fmt[strlen(format)+3];
  fmt[strlen(format)+2] = '\0';
  strcpy(fmt, format);
  strcpy(fmt+strlen(fmt), ", ");

  printf("[");
  for (list_node *n = l->head->next; n != l->tail; n = n->next) {
    if (n != l->tail->prev)
      printf(fmt, ((hashmap_entry *) n->e)->key);
    else
      printf(format, ((hashmap_entry *) n->e)->key);
  }
  printf("]");
  // Reset deletion on list so we don't accidentally free the data
  l->del = do_not_del;
  list_del(l);
}
// Print values of a hashmap
void map_values_print(const hashmap *m, char *format) {
  list *l = map_pairs(m);

  // Build the new format string
  char fmt[strlen(format)+3];
  fmt[strlen(format)+2] = '\0';
  strcpy(fmt, format);
  strcpy(fmt+strlen(fmt), ", ");

  printf("[");
  for (list_node *n = l->head->next; n != l->tail; n = n->next) {
    if (n != l->tail->prev)
      printf(fmt, ((hashmap_entry *) n->e)->value);
    else
      printf(format, ((hashmap_entry *) n->e)->value);
  }
  printf("]");
  // Reset deletion on list so we don't accidentally free the data
  l->del = do_not_del;
  list_del(l);
}
