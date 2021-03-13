#ifndef TREE_H
#define TREE_H

#include "stdlib.h"
#include "stdbool.h"

// Simple binary tree
typedef struct TreeNode {
    struct TreeNode *left;
    struct TreeNode *right;
    void *e;
} tree_node;

typedef struct Tree {
    tree_node *root;
    unsigned int len;
    void *(*copy) (const void *e);
    void (*del) (void *e);
} tree;

// Create a new node for the tree
static inline tree_node *tree_new_node(void* e) {
  tree_node *n = (tree_node*) malloc(sizeof(tree_node));
  n->left = n->right = NULL;
  n->e = e;
  return n;
}

// Construct a new tree
tree *tree_new(void *(*copy) (const void *e), void (*del) (void *e));

/* Destroy a tree and all elements inside */
// Used to free nodes in a tree
void tree_free_subnodes(tree *t, tree_node *n);
// Not exactly meant to be called directly, main logic behind destroying the tree
static inline void tree_del(tree *t) {
  // Set initial starting node
  tree_free_subnodes(t, t->root);
  // Walk down the left side
  free(t);
}

// Simple thing to add to the left of the tree and keep track of the length
static inline tree_node *tree_add_left(tree *t, tree_node *n, void* e) {
  ++t->len;
  return n->left = tree_new_node(e);
}
// Simple thing to add to the right of the tree and keep track of the length
static inline tree_node *tree_add_right(tree *t, tree_node *n, void* e) {
  ++t->len;
  return n->right = tree_new_node(e);
}

// Check to see if the node is a leaf
static inline bool tree_is_leaf(tree_node *n) {
  return (n->left == NULL && n->right == NULL);
}

// Calculate tree length from node in case we hadn't been keeping track properly
unsigned int tree_calc_len_from(tree *t, tree_node* n);
// Calculate tree length in case we hadn't been keeping track properly
static inline unsigned int tree_calc_len(tree *t) {
  return t->len = tree_calc_len_from(t, t->root);
}

// Get height of the tree from the current position
unsigned int tree_height_from(tree *t, tree_node *n);
// Get height of the tree
static inline unsigned int tree_height(tree *t) {
  return tree_height_from(t, t->root);
}

// Clone a tree from a certain node
tree_node *tree_copy_from(tree *t, tree_node *n);
// Clone a tree
static inline tree *tree_copy(tree *t) {
  tree *new_t = tree_new(t->copy, t->del);
  new_t->root = tree_copy_from(t, t->root);
  new_t->len = t->len;
  return new_t;
}

// Build a subtree
static inline tree_node *tree_make_from(void *e, tree_node *left, tree_node *right) {
  tree_node *n = tree_new_node(e);
  n->left = left;
  n->right = right;
  return n;
}

// A utility function to print preorder traversal of the avl_tree.
void tree_printr(tree_node *node, char *format);
static inline void tree_print(tree *t, char *format) {
  // Print 4 spaces between nodes
  tree_printr(t->root, format);
}
static inline void tree_println(tree *t, char *format) {
  // Print 4 spaces between nodes
  tree_printr(t->root, format);
  printf("\n");
}

#endif