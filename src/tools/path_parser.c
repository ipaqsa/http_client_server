#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "path_parser.h"

void parts_count(char *path, int *c, char s) {
    (*c) = path[0] == s ? 0 : 1;
    for (int i = 0; i < strlen(path); i++) {
        if (path[i] == s)
            (*c)++;
    }
}

void parts_lens(char *path, int **parts_ls, int c, char s) {
    *parts_ls = malloc(sizeof(int) * c);
    int n = -1;
    int count = 0;
    int i = 0;
    for (; i < strlen(path); i++) {
        if (path[i] == s && i != 0) {
            n++;
            (*parts_ls)[n] = count;
            count = 0;
            if (n == c - 2) {
                break;
            }
        } else {
            if (path[i] != s) count++;
        }
    }
    (*parts_ls)[c-1] = strlen(path) - i - 1;
}

void split_path(char *path, char ***parts, int *c, char s) {
    int *parts_ls = NULL;

    parts_count(path, c, s);
    *parts = malloc(sizeof(char*) * (*c));
    if (*c == 1) {
        (*parts)[0] = path;
        return;
    }
    parts_lens(path, &parts_ls, *c, s);
    for (int l = 0; l < *c; l++) {
        (*parts)[l] = malloc(sizeof(char ) * parts_ls[l]+1);
    }
    int n = 0;
    int idx = 0;
    for (int i = 0; i < strlen(path); i++) {
        if (idx == parts_ls[n]+1) {
            n++;
            (*parts)[n][idx] = '\0';
            idx = 0;
        }
        (*parts)[n][idx] = path[i];
        idx++;
    }
}

void add_path(tree_t *tree, char *path, void (*func)(int, HTTPreq*)) {
    char **parts;
    int c = 0;
    split_path(path, &parts, &c, '/');
    tree_t *tmp = tree;
    for (int i = 0; i < c; i++) {
        add_child(tmp, parts[i]);
        tmp = in_children(tmp, parts[i]);
    }
    set_func(tmp, func);
}

int exc_path(tree_t *tree, int conn, HTTPreq *request) {
    char **parts;
    int c = 0;
    split_path(request->path, &parts, &c, '/');
    tree_t *tmp = tree;
    for (int i = 0; i < c; i++) {
        if (strcmp(request->path, "/") == 0) {
            break;
        }
        tmp = in_children(tmp, parts[i]);
        if (tmp == NULL) {
            return 0;
        }
    }
    tmp->func(conn, request);
    return 1;
}