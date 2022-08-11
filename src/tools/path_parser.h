#ifndef HTTP_PATH_PARSER_H
#define HTTP_PATH_PARSER_H

#include "../ds/tree_for_parsing.h"
#include "../http.h"

void parts_count(char *path, int *c, char s);
void parts_lens(char *path, int **parts_ls, int c, char s);
void split_path(char *path, char ***parts, int *c, char s);
void add_path(tree_t *tree, char *path, void (*func)(int, HTTPreq*));
int exc_path(tree_t *tree, int conn, HTTPreq *request);

#endif
