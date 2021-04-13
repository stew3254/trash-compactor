#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "stdlib.h"
#include "stdbool.h"
#include "simple_functions.h"
#include "list.h"

typedef struct AVLTreeNode {
    struct AVLTreeNode *left;
    struct AVLTreeNode *right;
    void *e;
    unsigned int height;
} avl_tree_node;

typedef struct AVLTree {
    avl_tree_node *root;
    unsigned int height;
    unsigned int len;
    int (*cmp) (const void *a, const void *b);
    void *(*copy) (const void *e);
    void (*del) (void *e);
} avl_tree;

typedef struct AVLTreeSearchResult {
    bool found;
    void * e;
} search_result;

static inline avl_tree_node *avl_tree_new_node(void* e) {
  avl_tree_node *n = (avl_tree_node*) malloc(sizeof(avl_tree_node));
  n->left = n->right = NULL;
  n->e = e;
  n->height = 1;
  return n;
}

// Create a new AVL tree
avl_tree *avl_tree_new(
    int (*cmp) (const void *a, const void *b),
    void *(*copy) (const void *e),
    void (*del) (void *e)
);

/* Destroy a avl_tree and all elements inside */
// Used to free nodes in a avl_tree
void avl_tree_free_subnodes(avl_tree *t, avl_tree_node *n);
// Not exactly meant to be called directly, main logic behind destroying the avl_tree
static inline void avl_tree_del(avl_tree *t) {
  // Set initial starting node
  if (t->root != NULL)
    avl_tree_free_subnodes(t, t->root);
  // Walk down the left side
  free(t);
}

// Get height of node
static inline unsigned int avl_tree_node_height(avl_tree_node* n) {
  return n == NULL ? 0 : n->height;
}

// Get height of the avl_tree
unsigned int avl_tree_height(const avl_tree *t);

// Check to see if the node is a leaf
static inline bool avl_tree_is_leaf(avl_tree_node *n) { return (n->left == NULL && n->right == NULL); }
// Check to see if the avl_tree_is_empty
static inline bool avl_tree_is_empty(avl_tree *t) { return t->root == NULL; }

// Recursive function to help get max value of avl_tree
void *avl_tree_max_from(avl_tree_node *node);
// Get max value of node in avl_tree
static inline void *avl_tree_max(avl_tree *t) {
  return avl_tree_max_from(t->root);
}

// Recursive function to help get min value of avl_tree
void *avl_tree_min_from(avl_tree_node *node);
// Get min value of node in avl_tree
static inline void *avl_tree_min(avl_tree *t) {
  return avl_tree_min_from(t->root);
}

/* avl_tree rotations */
avl_tree_node *avl_tree_left_rotate(avl_tree_node *r);
avl_tree_node *avl_tree_right_rotate(avl_tree_node *r);

// Get Balance factor of node n
int avl_tree_get_node_balance(avl_tree_node *n);

// Recursive function to avl_tree_insert_from a key in the avl tree rooted
// with node and returns the new root of the avl tree.
avl_tree_node* avl_tree_insert_from(avl_tree *t, avl_tree_node* node, void *e);
// Simple wrapper for recursive function
static inline void avl_tree_insert(avl_tree *t, void *e) {
  t->root = avl_tree_insert_from(t, t->root, e);
  ++t->len;
  t->height = avl_tree_height(t);
}

// Recursive function to delete a node with given e
// from avl tree with given node. It returns node of
// the modified avl tree.
avl_tree_node* avl_tree_remove_from(avl_tree *t, avl_tree_node* node, void *e);
// Simple wrapper for recursive function
static inline void avl_tree_remove(avl_tree *t, void *e) {
  t->root = avl_tree_remove_from(t, t->root, e);
  --t->len;
  t->height = avl_tree_height(t);
}

// Find based on certain function and height
search_result avl_tree_get_from(avl_tree *t, avl_tree_node *n, const void *e);

// Wrapper for before but you can specify search function
static inline search_result avl_tree_get(avl_tree *t, const void *e) {
  return avl_tree_get_from(t, t->root, e);
}

// Converts the tree into a sorted list from a certain point in the tree
void avl_tree_to_list_from(const avl_tree *t, avl_tree_node *n, list *l, bool forward);
// Converts the tree into a sorted list
static inline list *avl_tree_to_list(const avl_tree *t) {
  list *l = list_new(t->cmp, t->copy, t->del);
  // Make sure we don't accidentally dereference a null pointer
  if (t->root != NULL)
    avl_tree_to_list_from(t, t->root, l, true);
  return l;
}
// Converts the tree into a reverse sorted list
static inline list *avl_tree_to_list_rev(const avl_tree *t) {
  list *l = list_new(t->cmp, t->copy, t->del);
  // Make sure we don't accidentally dereference a null pointer
  if (t->root != NULL)
    avl_tree_to_list_from(t, t->root, l, false);
  return l;
}

// Copy a tree from a certain point
avl_tree_node *avl_tree_copy_from(avl_tree *t, avl_tree_node *n);
// Copy a tree
static inline avl_tree *avl_tree_copy(avl_tree *t) {
  avl_tree *new_t = avl_tree_new(t->cmp, t->copy, t->del);
  new_t->root = avl_tree_copy_from(t, t->root);
  new_t->len = t->len;
  return new_t;
}

// A utility function to print preorder traversal of the avl_tree.
// The function also prints height of every node
void avl_tree_printr(avl_tree_node *node, unsigned int offset, char *format);
static inline void avl_tree_print(avl_tree *t, char *format) {
  // Print 4 spaces between nodes
  avl_tree_printr(t->root, t->height*4, format);
}
static inline void avl_tree_println(avl_tree *t, char *format) {
  // Print 4 spaces between nodes
  avl_tree_printr(t->root, t->height*4, format);
  printf("\n");
}

#endif