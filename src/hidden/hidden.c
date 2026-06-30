#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "hidden.h"
#include "stun/stun.h"

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

int hidden_send(struct hidden_server *hs, uint8_t data[12]) {
    memcpy(hs->sent_data, data, 12);
    for (int i = 0; i < hs->stuns_next_index; i++) {
        struct stun *s = &hs->stuns[i];
        for (int r = 0; r < 3; r++) {
            if (send_binding_request(hs->socket, s->address, s->port, data) < 0) {
                continue;
            } 

            break;
        }
    }

    return 0;
}

int hidden_read(struct hidden_server *hs, uint8_t data[12]) {
    char transaction[12];
    int n = read_transaction(hs->socket, transaction, NULL, NULL);
    if (n < 0) return n;

    if (memcmp(transaction, hs->sent_data, 12)) {
        memcpy(data, transaction, 12);
        return 0;
    }

    return -1;
}
