#include <stdio.h>
#include "simple_functions.h"
#include "tree.h"

// Construct a new tree
tree *tree_new(void *(*copy) (const void *e), void (*del) (void *e)) {
  tree *t = (tree*) malloc(sizeof(tree));
  t->root = NULL;
  t->len = 0;
  t->copy = copy;
  t->del = del;
  return t;
}

// Used to free nodes in a avl_tree
void tree_free_subnodes(tree *t, tree_node *n) {
  // Free left hand side
  if (n->left != NULL) {
    tree_free_subnodes(t, n->left);
  }
  // Free right hand side
  if (n->right != NULL) {
    tree_free_subnodes(t, n->right);
  }

  // Clean up current node
  t->del(n->e);
  free(n);
}

// Calculate tree length in case we hadn't been keeping track properly
unsigned int tree_calc_len_from(tree *t, tree_node *n) {
  unsigned int len = 0;
  if (n == NULL)
    return 0;
  if (n->left != NULL)
    len += tree_calc_len_from(t, n->left);
  if (n->right != NULL)
    len += tree_calc_len_from(t, n->right);
  return len + 1;
}

// Get height of the tree from the current position
unsigned int tree_height_from(tree *t, tree_node *n) {
  if (n == NULL)
    return 0;
  if (n->left == NULL)
    return tree_height_from(t, n->right) + 1;
  else if (n->right == NULL)
    return tree_height_from(t, n->left) + 1;
  return max(tree_height_from(t, n->left), tree_height_from(t, n->right)) + 1;
}

// Clone a tree from a certain node
tree_node *tree_copy_from(tree *t, tree_node *n) {
  if (n == NULL)
    return n;
  tree_node *new_n = tree_new_node(t->copy(n->e));
  new_n->left = tree_copy_from(t, n->left);
  new_n->right = tree_copy_from(t, n->right);
  return new_n;
}

// A utility function to print preorder traversal of the tree given a print function
void tree_print_withr(tree_node *node, void (p)(void *e)) {
  if(node != NULL) {
    tree_print_withr(node->left, p);
    p(node->e);
    tree_print_withr(node->right, p);
  }
}

// A utility function to print preorder traversal of the tree.
void tree_printr(tree_node *node, char *format) {
  if(node != NULL) {
    tree_printr(node->left, format);
    printf(format, node->e);
    tree_printr(node->right, format);
  }
}
