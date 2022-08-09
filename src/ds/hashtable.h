#ifndef HTTP_HASHTABLE_H
#define HTTP_HASHTABLE_H

#include "tree.h"
#include <stdlib.h>

typedef struct HashTable {
    struct {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    size_t size;
    Tree **table;
} HashTable;

extern HashTable *new_hashtab(size_t size, vtype_tree_t value);
extern void free_hashtab(HashTable *hashTable);

extern value_tree_t get_hashtab(HashTable *hashTable, void *key);
extern void set_hashtab(HashTable *hashTable, void *key, void *value);
extern void del_hashtab(HashTable *hashTable, void *key);
extern _Bool in_hashtab(HashTable *hashTable, void *key);

extern void print_hashtab(HashTable *hashTable);
#endif
