#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "net.h"
#include "http.h"

#define METHOD_SIZE 16
#define PATH_SIZE 2048
#define PROTO_SIZE 16

typedef struct HTTP {
    char *host;
    int32_t len;
    int32_t cap;
    void (**funcs)(int, HTTPreq*);
//    HashTab
} HTTP;

static void _null_request(HTTPreq *request);
static HTTPreq _new_request(void );
static void _parse_request(HTTPreq *request, char *buffer, size_t size);

extern HTTP *new_http(char *address) {
    HTTP *http = (HTTP*) malloc(sizeof(HTTP));
    http->cap = 500;
    http->len = 0;
    http->host = (char*) malloc(sizeof(char ) * strlen(address) + 1);
    strcpy(http->host, address);
    http->funcs = (void (**)(int, HTTPreq*)) malloc(http->cap * (sizeof(void (*)(int, HTTPreq*))));
    return http;
}

extern void free_http(HTTP *http) {
    free(http->host);
    free(http->funcs);
    free(http);
}

extern void handle_http(HTTP *http, char *path, void (*handle)(int, HTTPreq*)) {

}

extern int8_t listen_http(HTTP *http) {
    int listener = listen_http(http->host);
    if (listener < 0) {
        return 1;
    }
    while (1) {
        int conn = accept_net(listener);
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
//        _switch_http(http, conn, &req);
        close_net(conn);
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
    }
}

static void _null_request(HTTPreq *request) {
    request->state += 1;
    request->index = 0;
}