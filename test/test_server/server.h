#include <netinet/in.h>

#define BUFFER_SIZE 16000

struct Server {
    int32_t domain;
    int32_t port;
    int32_t service;
    int32_t protocol;
    int32_t backlog;
    uint64_t interface;

    int32_t socket;
    struct sockaddr_in address;

    void (*launch)(struct Server *server);
};

struct Server server_Constructor(int32_t domain, int32_t port, int32_t service, int32_t protocol, int32_t backlog, uint64_t interface, void (*launch)(struct Server *server));
void launch(struct Server *server);

#pragma once