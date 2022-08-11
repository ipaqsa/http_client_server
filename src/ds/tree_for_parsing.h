#ifndef HTTP_TREE_FOR_PARSING_H
#define HTTP_TREE_FOR_PARSING_H

#include "../http.h"

typedef struct node {
    struct node *parent;
    char *key;
    void (*func)(int, HTTPreq*);
    int children_count;
    struct node **children;
} tree_t;

tree_t *init_tree(char *key, void (*func)(int, HTTPreq*));
void tree_f(tree_t *tree);
void add_child(tree_t *tree, char *key);
void set_func(tree_t *tree, void (*func)(int, HTTPreq*));
tree_t *in_children(tree_t *tree, char *key);

#endif
