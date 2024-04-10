#ifndef CONNECT_H
#define CONNECT_H

#include "http.h"
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <poll.h>

#define MAX_CLIENTS 256
#define PORT 8080
#define LISTEN_QUEUE 50

typedef struct sockaddr_in ServerAddress;

typedef enum {
    STATE_NEW,
    STATE_CONNECTED,
    STATE_DISCONNECTED,
} State;

typedef struct {
    int fd;
    State state;
    Request request;
    char *addr;
    uint16_t port;
} Client;

typedef struct {
    int listen_fd;
    int conn_fd;
    int opt;
    int nfds;
    Client clients[MAX_CLIENTS];
    struct pollfd fds[MAX_CLIENTS+1];
    ServerAddress addr;
    ServerAddress client_addr;
} Server;

int find_free_slot(Server server);
int find_slot_by_fd(Server server, int fd);

Server init_server(void);
Server start_server(Server *server);

#endif // !CONNECT_H    
