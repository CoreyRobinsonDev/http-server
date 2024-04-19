#include "include/http.h"
#include "include/style.h"
#include "include/connect.h"
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>


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

    for (int i = 0; i < MAX_CLIENTS; i++) {
        server.clients[i].fd = -1;
        server.clients[i].state = STATE_NEW;
    }

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
    server.addr.sin_addr.s_addr = ADDRESS;
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

    char str[50];
    snprintf(str, sizeof(str), "Server listening on port %d...", PORT);
    print_info(INFO, str);

    server.fds[0].fd = server.listen_fd;
    server.fds[0].events = POLLIN;


    return server;
}

bool accept_client(Server *server) {
    if (!(server->fds[0].revents & POLLIN)) return false;

    socklen_t client_len = sizeof(server->client_addr);
    if ((server->conn_fd = accept(
        server->listen_fd,
        (struct sockaddr*)&server->client_addr,
        &client_len
    )) == -1) {
        perror("accept");
    }

    return true;
}

void connect_client(Server *server) {
    int free_slot = find_free_slot(*server);
    if (free_slot == -1) {
        // TODO: Add real response
        Response res = {
            .header = {0},
            .body = "Server Full"
        };
        if (send(server->conn_fd, &res, sizeof(res), MSG_CONFIRM) == -1) {
            perror("send");
        }
        close(server->conn_fd);
    } else {
        server->clients[free_slot].fd = server->conn_fd;
        server->clients[free_slot].state = STATE_CONNECTED;
        server->clients[free_slot].addr = inet_ntoa(
            server->client_addr.sin_addr
        );
        server->clients[free_slot].port = ntohs(
            server->client_addr.sin_port
        );
        server->fds[free_slot+1].fd = server->clients[free_slot].fd;
        server->fds[free_slot+1].events = POLLIN;
        server->nfds++;
    }
}

Server handle_client(Server *server, int socket) {
    int slot = find_slot_by_fd(*server, server->fds[socket].fd);
    if (slot == -1) {
        char str[50];
        snprintf(str, sizeof(str), "No fd found at socket %d\n", socket);
        print_info(ERROR, str);
        return *server;
    }

    char term_out[50];
    char req_buf[(BUFF_SIZE*2)+1] = {0};

    ssize_t req_buffer_size = recv(
        server->fds[socket].fd,
        &req_buf,
        sizeof(req_buf) - 1,
        0
    );

    Request req = parse_req(req_buf);
    Response res = {
        .header = {0},
        .body = "Hello, World!"
    };

    if (req_buffer_size <= 0) {
        snprintf(term_out, sizeof(term_out), 
            "%s:%d has disconnected\n", 
            server->clients[slot].addr,
            server->clients[slot].port
        );
        print_info(INFO, term_out);

        close(server->fds[socket].fd);
        server->clients[slot].fd = -1;
        server->clients[slot].state = STATE_DISCONNECTED;
        memset(&server->clients[slot].addr, '\0', sizeof(server->clients[slot].addr));
        server->clients[slot].port = 0;
        server->nfds--;
    } else {
        printf(
            "\t\t===[%s:%d]===\n",
            server->clients[slot].addr,
            server->clients[slot].port
        );
        printf("%s\n", req_buf);
        printf(
            "\t\t===[%s:%d]===\n",
            server->clients[slot].addr,
            server->clients[slot].port
        );
        if (send(server->clients[slot].fd, &res, sizeof(res), MSG_CONFIRM) == -1) {
            perror("send");
        }
        close(server->clients[slot].fd);
    } 
    memset(&req_buf, '\0', sizeof(req_buf));

    return *server;
}

Server start_server(Server *server) {
    server->nfds = 1; 

    while(true) {
        int n_events = poll(server->fds, server->nfds, -1);
        if (n_events == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (accept_client(server)) {
            connect_client(server);
            n_events--;
        }

        for (int i = 1; i <= server->nfds && n_events > 0; i++) {
            if (server->fds[i].revents & POLLIN) {
                n_events--;
                *server = handle_client(server, i);
            }
        }
    }

    return *server;
}
























