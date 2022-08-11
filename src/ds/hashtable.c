#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "hashtable.h"

static uint32_t _strhash(uint8_t *s, size_t size) {
    uint32_t hashval;
    for (hashval = 0; *s != '\0' ; ++s) {
        hashval = *s + 31 * hashval;
    }
    return hashval % size;
}

extern HashTable *new_hashtab(size_t size, vtype_tree_t value) {
    switch (value) {
        case DECIMAL_ELEM:
        case STRING_ELEM:
        case REAL_ELEM:
            break;
        default:
            fprintf(stderr, "Value type not supported");
            return NULL;
    }
    HashTable *hashTable = (HashTable*) malloc(sizeof(HashTable));
    hashTable->table = (Tree**) malloc(sizeof(Tree) * size);
    for (size_t i = 0; i < size; i++) {
        hashTable->table[i] = new_tree(STRING_ELEM, value);
    }
    hashTable->size = size;
    hashTable->type.value = value;
    hashTable->type.key = STRING_ELEM;
    return hashTable;
}

extern void free_hashtab(HashTable *hashTable) {
    for (size_t i = 0; i < hashTable->size; i++) {
        free_tree(hashTable->table[i]);
    }
    free(hashTable->table);
    free(hashTable);
}

extern value_tree_t get_hashtab(HashTable *hashTable, void *key) {
    uint32_t hash = _strhash(key, hashTable->size);
    value_tree_t result = get_tree(hashTable->table[hash], key);
    return result;
}

extern void set_hashtab(HashTable *hashTable, void *key, void *value) {
    uint32_t hash = _strhash(key, hashTable->size);
    set_tree(hashTable->table[hash], key, value);
}

extern void del_hashtab(HashTable *hashTable, void *key) {
    uint32_t hash = _strhash(key, hashTable->size);
    del_tree(hashTable->table[hash], key);
}

extern _Bool in_hashtab(HashTable *hashTable, void *key) {
    uint32_t hash = _strhash(key, hashTable->size);
    _Bool result;

    result = in_tree(hashTable->table[hash], key);
    return result;
}

extern void print_hashtab(HashTable *hashTable) {
    uint32_t hash;
    printf("{\n");
    for (size_t i = 0; i < hashTable->size; i++) {
        if (hashTable->table[i]->node == NULL) {
            continue;
        }
        hash = _strhash(hashTable->table[i]->node->data.key.string, hashTable->size);
        printf("\t%d =>", hash);
        print_tree_as_list(hashTable->table[i]);
    }
    printf("}\n");
}