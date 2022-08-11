#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree_for_parsing.h"

tree_t *new_node(char *key) {
    tree_t *tree = malloc(sizeof(tree_t));
    tree->children = malloc(sizeof(tree_t*) * 70);
    tree->parent = NULL;
    tree->key = key;
    tree->children_count = 0;
    return tree;
}

tree_t *init_tree(char *key, void (*func)(int, HTTPreq*)) {
    tree_t *tree = malloc(sizeof(tree_t));
    tree->children = malloc(sizeof(tree_t*) * 70);
    tree->parent = NULL;
    tree->key = key;
    tree->children_count = 0;
    tree->func = func;
    return tree;
}

void tree_f(tree_t *tree) {
    for (int i = 0; i < 70; i++) {
        free(tree->children[i]);
    }
    free(tree);
}

void add_child(tree_t *tree, char *key) {
    tree_t *tmp = new_node(key);
    tmp->parent = tree;
    tree->children[tree->children_count] = tmp;
    tree->children_count++;
}

tree_t *in_children(tree_t *tree, char *key) {
    for (int i = 0; i < tree->children_count; i++) {
        if (strcmp(tree->children[i]->key, key) == 0) {
            return tree->children[i];
        }
    }
    return NULL;
}

void set_func(tree_t *tree, void (*func)(int, HTTPreq*)) {
    tree->func = func;
}
