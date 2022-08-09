#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "net.h"

void server() {
    int listener = listen_net("127.0.0.1:8080");
    if (listener < 0) {
        fprintf(stderr, "%d\n", listener);
        exit(1);
    }
    printf("Server is listening...\n");
    while (1) {
        printf("Wait...\n");
        int conn = accept_net(listener);
        if (conn < 0) {
            fprintf(stderr, "Error accept %d\n", conn);
            exit(3);
        }
        printf("Connect!!!");
        close_net(conn);
    }
    close_net(listener);
    return;
}