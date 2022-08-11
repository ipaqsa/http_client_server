#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "http.h"

void index_page(int conn, HTTPreq *request);
void about_page(int conn, HTTPreq *request);

void server(char *port) {
    char *addr = "127.0.0.1:";
    int port_len = strlen(port);
    int addr_len = strlen(addr);
    int full = port_len + addr_len + 1;
    char *host = malloc(sizeof(char ) * full);

    strcat(host, addr);
    strcat(host, port);
    HTTP *server = new_http(host);
    printf("> Server was started at %s\n",  host);
    handle_http(server, "/", index_page);
    handle_http(server, "/about", about_page);
    handle_http(server, "/about/21", about_page);

    listen_http(server);
    free_http(server);
}

void index_page(int conn, HTTPreq *request) {
    printf("> Load Index page\n");
//    if (strcmp(request->path, "/") != 0) {
//        printf("> Page not found\n");
//        parsehtml_http(conn, "../data/404.html");
//        return;
//    }
    parsehtml_http(conn, "../data/index.html");
}

void about_page(int conn, HTTPreq *request) {
    printf("> Load about page\n");
    parsehtml_http(conn, "../data/about.html");
}