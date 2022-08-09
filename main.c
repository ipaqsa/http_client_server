#include <stdio.h>
//#include "src/server.h"
//#include "src/client.h"
#include "src/ds/hashtable.h"

int main() {
//    char choice;
//    scanf("%c", &choice);
//    if (choice == 'c') {
//        printf("Start client...\n");
//        client();
//    }
//    else if (choice == 's') {
//        printf("Start server...\n");
//        server();
//    }

    HashTable *ht = new_hashtab(1000, DECIMAL_ELEM);
    set_hashtab(ht, string("Stefan"), decimal(5));
    set_hashtab(ht, string("Stefam"), decimal(2));
    print_hashtab(ht);
    return 0;
}
