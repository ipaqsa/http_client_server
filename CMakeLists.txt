cmake_minimum_required(VERSION 3.23)
project(HTTP C)

set(CMAKE_C_STANDARD 17)

add_executable(HTTP main.c src/server.c src/server.h src/client.c src/client.h src/net.c src/net.h src/http.c src/http.h src/ds/hashtable.c src/ds/hashtable.h src/ds/tree.c src/ds/tree.h)
