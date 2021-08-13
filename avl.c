/*
 * NOTE: Much of this code was taking from geeksforgeeks.org and then
 * adapted to fit my paradigm. I do not take credit for this algorithm's
 * implementation, only for the structuring of the functions. The only thing
 * I wrote myself thus far is the BFS style avl_tree print
 */
#include <stdio.h>
#include "simple_functions.h"
#include "avl.h"

avl_tree *avl_tree_new(
    int (*cmp) (const void *a, const void *b),
    void *(*copy) (const void *e),
    void (*del) (void *e)
) {
  avl_tree *t = (avl_tree*) malloc(sizeof(avl_tree));
  t->root = NULL;
  t->height = 0;
  t->len = 0;
  t->cmp = cmp;
  t->copy = copy;
  t->del = del;
  return t;
}

//Used to free nodes in a avl_tree
void avl_tree_free_subnodes(avl_tree *t, avl_tree_node *n) {
  //Free left hand side
  if (n->left != NULL) {
    avl_tree_free_subnodes(t, n->left);
  }
  //Free right hand side
  if (n->right != NULL) {
    avl_tree_free_subnodes(t, n->right);
  }

  //Clean up current node
  t->del(n->e);
  free(n);
}

// Get height of the avl_tree
unsigned int avl_tree_height(const avl_tree *t) {
  if (avl_tree_is_empty(t))
    return 0;

  unsigned int height;
  // Find the height of the avl_tree
  for (height = (sizeof(int) * 8) - 1; height > 0; --height) {
    // Found the significant bit in our length
    if ((t->len & (1 << height)) > 0)
      break;
  }

  return height + 1;
}

// Find the maximum value in the avl_tree
void *avl_tree_max_from(avl_tree_node *node) {
  avl_tree_node* current = node;

  /* loop down to find the leftmost leaf */
  while (current->right != NULL)
    current = current->right;

  return current->e;
}

// Find the minimum value in the avl_tree
void *avl_tree_min_from(avl_tree_node *node) {
  avl_tree_node* current = node;

  /* loop down to find the leftmost leaf */
  while (current->left != NULL)
    current = current->left;

  return current->e;
}

// Get Balance factor of node n
int avl_tree_get_node_balance(avl_tree_node *n) {
  if (n == NULL)
    return 0;
  return (int) avl_tree_node_height(n->right) - (int) avl_tree_node_height(n->left);
}


// A utility function to left rotate subtree rooted with r
// See the diagram given above.
avl_tree_node *avl_tree_left_rotate(avl_tree_node *r) {
  avl_tree_node *x = r->right;
  avl_tree_node *y = x->left;

  // Perform rotation
  x->left = r;
  r->right = y;

  //  Update heights
  r->height -= 2;

  // Return new root
  return x;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
avl_tree_node *avl_tree_right_rotate(avl_tree_node *r) {
  avl_tree_node *x = r->left;
  avl_tree_node *y = x->right;

  // Perform rotation
  x->right = r;
  r->left = y;

  // Update heights
  r->height -= 2;

  // Return new root
  return x;
}

// Recursive function to avl_tree_insert_from a key in the subtree rooted
// with node and returns the new root of the subtree.
avl_tree_node* avl_tree_insert_from(avl_tree *t, avl_tree_node* node, void *e) {
  /* 1.  Perform the normal BST insertion */
  if (node == NULL) {
    return avl_tree_new_node(e);
  }

  //Insert left in the avl_tree
  if (t->cmp(e, node->e) < 0) {
    node->left  = avl_tree_insert_from(t, node->left, e);
  } else if (t->cmp(e, node->e) > 0) {
    //Insert right in the avl_tree
    node->right = avl_tree_insert_from(t, node->right, e);
  } else {
    //We cannot have duplicates, so update it
    t->del(node->e);
    node->e = e;
    return node;
  }

  /* 2. Update height of this ancestor node */
  node->height = max(
      (void *) avl_tree_node_height(node->left),
      (void *) avl_tree_node_height(node->right)
  ) + 1;

  /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
  int balance = avl_tree_get_node_balance(node);

  // If this node becomes unbalanced, then
  // there are 4 cases

  // Left Left Case
  if (balance < -1 && t->cmp(e, node->left->e) < 0)
    return avl_tree_right_rotate(node);

  // Right Right Case
  if (balance > 1 && t->cmp(e, node->right->e) > 0)
    return avl_tree_left_rotate(node);

  // Left Right Case
  if (balance < -1 && t->cmp(e, node->left->e) > 0) {
    node->left = avl_tree_left_rotate(node->left);
    return avl_tree_right_rotate(node);
  }

  // Right Left Case
  if (balance > 1 && t->cmp(e, node->right->e) < 0) {
    node->right = avl_tree_right_rotate(node->right);
    return avl_tree_left_rotate(node);
  }

  /* return the (unchanged) node pointer */
  return node;
}

// Recursive function to delete a node with given e
// from subtree with given node. It returns node of
// the modified subtree.
avl_tree_node* avl_tree_remove_from(avl_tree *t, avl_tree_node* node, void *e) {
  // STEP 1: PERFORM STANDARD BST DELETE
  if (node == NULL)
    return node;

  // If the e to be deleted is smaller than the
  // node's e, then it lies in left subtree
  if (t->cmp(e, node->e) < 0) {
    node->left = avl_tree_remove_from(t, node->left, e);
  } else if(t->cmp(e, node->e) > 0) {
    // If the e to be deleted is greater than the
    // node's e, then it lies in right subtree
    node->right = avl_tree_remove_from(t, node->right, e);
  } else {
    // if e is same as node's e, then This is
    // the node to be deleted
    // node with only one child or no child
    if ((node->left == NULL) || (node->right == NULL)) {
      avl_tree_node *temp = node->left ? node->left : node->right;

      // No child case
      if (temp == NULL) {
        temp = node;
        node = NULL;
      } else {
        // One child case
        // Copy the contents of the non-empty child
        *node = *temp;
      }
      // Handle deletion of the node's element
      t->del(temp->e);
      // Delete the node
      free(temp);
    } else {
      // node with two children: Get the inorder
      // successor (smallest in the right subtree)
      // Copy the inorder successor's data to this node
      node->e = avl_tree_min_from(node->right);

      // Delete the inorder successor
      node->right = avl_tree_remove_from(t, node->right, node->e);
    }
  }

  // If the avl_tree had only one node then return
  if (node == NULL)
    return node;

  // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
  node->height = max(
      (void *) avl_tree_node_height(node->left),
      (void *) avl_tree_node_height(node->right)
  ) + 1;

  // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
  // check whether this node became unbalanced)
  int balance = avl_tree_get_node_balance(node);

  // If this node becomes unbalanced, then there are 4 cases

  // Left Left Case
  if (balance < -1 && avl_tree_get_node_balance(node->left) <= 0)
    return avl_tree_right_rotate(node);

  // Left Right Case
  if (balance < -1 && avl_tree_get_node_balance(node->left) > 0) {
    node->left =  avl_tree_left_rotate(node->left);
    return avl_tree_right_rotate(node);
  }

  // Right Right Case
  if (balance > 1 && avl_tree_get_node_balance(node->right) >= 0)
    return avl_tree_left_rotate(node);

  // Right Left Case
  if (balance > 1 && avl_tree_get_node_balance(node->right) < 0) {
    node->right = avl_tree_right_rotate(node->right);
    return avl_tree_left_rotate(node);
  }

  return node;
}

// Find based on certain function and height
search_result avl_tree_get_from(avl_tree *t, avl_tree_node *n, const void *e) {
  if (n == NULL) {
    search_result r = { .found = false };
    return r;
  }

  if (t->cmp(e, n->e) < 0) {
    return avl_tree_get_from(t, n->left, e);
  } else if (t->cmp(e, n->e) > 0) {
    return avl_tree_get_from(t, n->right, e);
  } else {
    search_result r = { .found = true, .e = n->e};
    return r;
  }
}

// Converts the tree into a sorted list
void avl_tree_to_list_from(const avl_tree *t, avl_tree_node *n, list *l, bool forward) {
  if (forward) {
    // Descend left subtree
    if (n->left != NULL)
      avl_tree_to_list_from(t, n->left, l, forward);
    // Add the current element
    list_push_back(l, n->e);
    // Descend right subtree
    if (n->right != NULL)
      avl_tree_to_list_from(t, n->right, l, forward);
  } else {
    // Descend right subtree
    if (n->right != NULL)
      avl_tree_to_list_from(t, n->right, l, forward);
    // Add the current element
    list_push_back(l, n->e);
    // Descend left subtree
    if (n->left != NULL)
      avl_tree_to_list_from(t, n->left, l, forward);
  }
}

// Copy a tree from a certain point
avl_tree_node *avl_tree_copy_from(avl_tree *t, avl_tree_node *n) {
  if (n == NULL)
    return n;
  avl_tree_node *new_n = avl_tree_new_node(t->copy(n->e));
  new_n->left = avl_tree_copy_from(t, n->left);
  new_n->right = avl_tree_copy_from(t, n->right);
  return new_n;
}

// A utility function to print preorder traversal of the avl_tree.
// The function also prints height of every node
void avl_tree_printr(avl_tree_node *node, unsigned int offset, char *format) {
  if(node != NULL) {
    printf(format, node->e);
    avl_tree_printr(node->left, offset - 4, format);
    avl_tree_printr(node->right, offset - 4, format);
  }
}