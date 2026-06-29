#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../stun/stun.h"

struct stun {
    uint32_t address;
    uint16_t port;
};

struct hidden_server {
    int socket; // udp socket

    struct stun *stuns; // stuns array
    size_t stuns_length; // max length
    size_t stuns_next_index; // next index
};

int init_hidden_server(struct hidden_server *hs, size_t length) {
    if (length < 1) return -1;

    hs->socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (hs->socket < 0) return hs->socket;

    hs->stuns = malloc(sizeof(struct stun)*length);
    if (!hs->stuns) {
        close(hs->socket);
        return -1;
    }

    hs->stuns_next_index = 0;
    hs->stuns_length = length;

    return 0;
}

void deinit_hidden_server(struct hidden_server *hs) {
    close(hs->socket);
    free(hs->stuns);
}

int add_stun(struct hidden_server *hs, uint8_t *addr, uint16_t port) {
    if (hs->stuns_next_index >= hs->stuns_length) return -1;

    uint32_t address = ntohl(inet_addr(addr));
    if (address == 0xFFFFFFFF) return -1;

    hs->stuns[hs->stuns_next_index] = (struct stun){
        .address = address,
        .port = port
    };
    hs->stuns_next_index++;

    return 0;
}

int send_hidden_server(struct hidden_server *hs, uint8_t data[12]) {
    for (int i = 0; i < hs->stuns_next_index; i++) {
        struct stun *s = &hs->stuns[i];
        for (int r = 0; r < 3; i++) {
            if (send_binding_request(hs->socket, s->address, s->port, data) < 0) {
                continue;
            } 

            break;
        }
    }

    return 0;
}

int main() {
    struct hidden_server hs;
    init_hidden_server(&hs, 3);

    add_stun(&hs, "23.21.150.121", 3478);
    add_stun(&hs, "18.191.223.12", 3478);
    add_stun(&hs, "127.0.0.1", 3478);

    uint8_t data[12] = "ping";
    send_hidden_server(&hs, data);

    uint32_t addr;
    uint16_t port;
    uint8_t reply[12];
    while (1) {
        int n = read_transaction(hs.socket, reply, &addr, &port);
        if (n < 0) continue;

        if (memcmp(data, reply, 12)) {
            printf("%s\n", (char *)reply);
        }
    }
}
