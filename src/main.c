#include <stdio.h>
#include "string.h"
#include "hashmap.h"

int main() {
  hashmap *m = map_new(map_simple_entry_copy, free);

  char *s = "Fuck this shit";
  char *foo = "Fuck this shit";
  long r;
  map_insert(m, foo, sizeof(char), strlen(s), 5);
  if (map_get(m, s, sizeof(char), strlen(foo), &r) == -1)
    printf("This is bad\n");
  else
    printf("a = %ld\n", r);
}

//int main() {
//  tree *t = tree_new(return_elem, do_not_del);
//  tree_node *n;
//  for (int i = 0; i < 20; ++i) {
//    if (t->root == NULL) {
//      n = t->root = tree_new_node(i+1);
//    } else if (i % 2 == 0) {
//      if (n->right != NULL)
//        n = n->right;
//      n->right = tree_new_node(i+1);
//    } else {
//      if (n->left != NULL)
//        n = n->left;
//      n->left = tree_new_node(i+1);
//    }
//  }
////  for (int i = 0; i < 5; ++i) {
////    if (t->root == NULL) {
////      n = t->root = tree_new_node(i+1);
////    } else {
////      n->right = tree_new_node(i+1);
////      n = n->right;
////    }
////  }
//
//  tree_println(t, "%d ");
//
//  tree *t2 = tree_copy(t);
//  tree_println(t2, "%d ");
//  tree_calc_len(t);
//  printf("%d %d\n", tree_height(t), t->len);
//
//  tree_del(t);
//  tree_del(t2);
//}