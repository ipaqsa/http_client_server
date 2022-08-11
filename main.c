#include <stdio.h>
#include <stdlib.h>
#include "src/server.h"
#include "src/client.h"
#include <termio.h>

#define RED "\x1B[31m"
#define RST "\x1B[0m"

void print_menu(int state) {
    if (state == 1) {
        system("clear");
        printf(RED);
        printf("=> 1. Server\n");
        printf(RST);
        printf("2. Client\n");
        printf("3. Exit\n");
    }
    else if (state == 2) {
        system("clear");
        printf("1. Server\n");
        printf(RED);
        printf("=> 2. Client\n");
        printf(RST);
        printf("3. Exit\n");
    }
    else if (state == 3) {
        system("clear");
        printf("1. Server\n");
        printf("2. Client\n");
        printf(RED);
        printf("=> 3. Exit\n");
        printf(RST);
    }
}

int choice() {
    char ch;
    int state = 1;
    print_menu(state);
    while (ch != 'q') {
        ch = getchar();
        if (ch == '\033') {
            getchar();
            switch(getchar()) {
                case 'A':
                    if (state == 1) {
                        state = 3;
                        break;
                    }
                    else if (state == 2) {
                        state = 1;
                        break;
                    }
                    else
                        state = 2;
                    break;
                case 'B':
                    if (state == 1) {
                        state = 2;
                        break;
                    }
                    else if (state == 2) {
                        state = 3;
                        break;
                    }
                    else
                        state = 1;
                    break;
            }
        }
        print_menu(state);
        if (ch == 10) {
            system("clear");
            return state;
        }
    }
}
//int main() {
//    struct termios term_o, term_n;
//    tcgetattr(fileno(stdin), &term_o);
//    term_n = term_o;
//    term_n.c_lflag &= ~ECHO;
//    term_n.c_lflag &= ~ICANON;
//    term_n.c_cc[VMIN] = 1;
//    term_n.c_cc[VTIME] = 0;
//    term_n.c_iflag &= ~ISIG;
//    tcsetattr(fileno(stdin), TCSANOW, &term_n);
//
//    int cho = choice();
//    tcsetattr(fileno(stdin), TCSANOW, &term_o);
//    if (cho == 1) {
//        printf("> Enter port: ");
//        char port[6];
//        scanf("%s", port);
//        server("5000");
//    }
//    else if (cho == 2) {
//        client();
//    }
//    return 0;
//}

int main() {
//    server("5000");
    client();
    return 0;
}