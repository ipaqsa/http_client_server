#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "net.h"
#include "client.h"
#include "arpa/inet.h"

typedef struct HTTPreq {
    char method[16];
    char path[2048];
    char proto[16];
} HTTPreq;


char *get_addr(char *address) {
    int status;
    struct addrinfo hints, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(address, NULL, &hints, &res);
    for(p = res;p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            printf("<%d>", ipv4->sin_port);
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf(" %s: %s\n", ipver, ipstr);
        return ipstr;
    }
    freeaddrinfo(res);
}


void request(int conn) {
    char buffer[BUFSIZ] = "GET / HTTP/1.1\nHost: 190.115.31.66:80 \nConnection: keep-alive\n\n";
    size_t readsize = strlen(buffer);
    printf("%s", buffer);
    send_net(conn, buffer, readsize);
}


void client(char *path) {
    char *address = get_addr("www.amedia.online");
    int conn = connect_net(address);
    printf("Connect success\n");
    request(conn);
    printf("Send\n");
    char buffer[2048];
    recv_net(conn, buffer, 2048);
    printf("%s", buffer);
}