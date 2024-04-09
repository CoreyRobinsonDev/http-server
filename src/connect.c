#include "include/connect.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>


// Return free slot or -1 if server is full
int find_free_slot(Server server) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server.clients[i].fd == -1) {
            return i;
        }
    }
    return -1;
}

// Return slot of given fd on server or -1 if not found
int find_slot_by_fd(Server server, int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server.clients[i].fd == fd) {
            return i;
        }
    }
    return -1;
}

Server init_server() {
    Server server = {0};
    server.opt = 1;

    if ((server.listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(
        server.listen_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &server.opt,
        sizeof(server.opt)
    ) == -1) {
        perror("setsocketopt");
        exit(EXIT_FAILURE);
    }

    server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = INADDR_ANY;
    server.addr.sin_port = htons(PORT);

    if (bind(
        server.listen_fd,
        (struct sockaddr*)&server.addr, 
        sizeof(server.addr)
    ) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server.listen_fd, LISTEN_QUEUE) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    server.fds[0].fd = server.listen_fd;
    server.fds[0].events = POLLIN;


    return server;
}
