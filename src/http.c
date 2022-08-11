#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "net.h"
#include "http.h"
#include "ds/hashtable.h"
#include "tools/path_parser.h"
#include "ds/tree_for_parsing.h"

#define METHOD_SIZE 16
#define PATH_SIZE 2048
#define PROTO_SIZE 16

typedef struct HTTP {
    char *host;
    int32_t len;
    int32_t cap;
    tree_t *tree;
    void (**funcs)(int, HTTPreq*);
    HashTable *tab;
} HTTP;

static void _null_request(HTTPreq *request);
static HTTPreq _new_request(void );
static void _parse_request(HTTPreq *request, char *buffer, size_t size);
static void _page404_http(int con);
static int8_t _switch_http(HTTP *http, int con, HTTPreq *request);


extern HTTP *new_http(char *address) {
    HTTP *http = (HTTP*) malloc(sizeof(HTTP));
    http->cap = 500;
    http->len = 0;
    http->host = (char*) malloc(sizeof(char ) * strlen(address) + 1);
    strcpy(http->host, address);
    http->tab = new_hashtab(http->cap, DECIMAL_ELEM);
    http->funcs = (void (**)(int, HTTPreq*)) malloc(http->cap * (sizeof(void (*)(int, HTTPreq*))));
    http->tree = init_tree("/", NULL);
    return http;
}

extern void free_http(HTTP *http) {
    free_hashtab(http->tab);
    tree_f(http->tree);
    free(http->host);
    free(http->funcs);
    free(http);
}

extern void handle_http(HTTP *http, char *path, void (*handle)(int, HTTPreq*)) {
//    set_hashtab(http->tab, string(path), decimal(http->len));
//    http->funcs[http->len] = handle;
//    http->len += 1;
//    if (http->len == http->cap) {
//        http->cap <<= 1;
//        http->funcs = (void(**)(int, HTTPreq*)) realloc(http->funcs, http->cap * sizeof(void(*)(int, HTTPreq*)));
//    }
    if (strcmp(path, "/") == 0) {
        set_func(http->tree, handle);
        return;
    }
    else {
        add_path(http->tree, path, handle);
    }
}

extern int8_t listen_http(HTTP *http) {
    int listener = listen_net(http->host);
    if (listener < 0) {
        return 1;
    }
    while (1) {
        int conn = accept_net(listener);
        printf("> Client accept...\n");
        if (conn < 0) {
            continue;
        }
        HTTPreq req = _new_request();
        while (1) {
            char buffer[BUFSIZ] = {0};
            int n = recv_net(conn, buffer, BUFSIZ);
            if (n < 0) {
                break;
            }
            _parse_request(&req, buffer, n);
            if (n != BUFSIZ) {
                break;
            }
        }
        _switch_http(http, conn, &req);
        close_net(conn);
        printf("> Answer was sent...\n");
    }
    close_net(listener);
    return 0;
}

extern void parsehtml_http(int conn, char *filename) {
    char buffer[BUFSIZ] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    while ((readsize = fread(buffer, sizeof(char), BUFSIZ, file)) != 0) {
        send_net(conn, buffer, readsize);
    }
    fclose(file);
}

static HTTPreq _new_request(void ) {
    return (HTTPreq) {
        .method = {0},
        .path = {0},
        .proto = {0},
        .state = 0,
        .index = 0
    };
}

static void _parse_request(HTTPreq *request, char *buffer, size_t size) {
    printf("%s\n", buffer);
    for (size_t i = 0; i < size; i++) {
        switch (request->state) {
            case 0:
                if (buffer[i] == ' ' || request->index == METHOD_SIZE -1) {
                    request->method[request->index] = '\0';
                    _null_request(request);
                    continue;
                }
                request->method[request->index] = buffer[i];
                break;
            case 1:
                if (buffer[i] == ' ' || request->index == PATH_SIZE -1) {
                    request->path[request->index] = '\0';
                    _null_request(request);
                    continue;
                }
                request->path[request->index] = buffer[i];
                break;
            case 2:
                if (buffer[i] == '\n' || request->index == PROTO_SIZE -1) {
                    request->proto[request->index] = '\0';
                    _null_request(request);
                    continue;
                }
                request->proto[request->index] = buffer[i];
                break;
            default:
                return;
        }
        request->index += 1;
    }
}

static void _null_request(HTTPreq *request) {
    request->state += 1;
    request->index = 0;
}

static int8_t _switch_http(HTTP *http, int con, HTTPreq *request) {
    if (exc_path(http->tree, con, request) == 0) {
        _page404_http(con);
    }
    else {
        return 1;
    }
}

static void _page404_http(int con) {
    printf("> Page not found\n");
    char *header = "HTTP/1.1 404 Not Found\nContent-type: text/html\n\n<h2>Not Found<h2>";
    size_t headsize = strlen(header);
    send_net(con, header, headsize);
}