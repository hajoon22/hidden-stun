#ifndef HIDDEN_H
#define HIDDEN_H

#include <stdint.h>

struct stun {
    uint32_t address;
    uint16_t port;
};

struct hidden_server {
    int socket; // udp socket

    uint8_t sent_data[12]; // sent transaction

    struct stun *stuns; // stuns array
    size_t stuns_length; // max length
    size_t stuns_next_index; // next index
};

int init_hidden_server(struct hidden_server *hs, size_t length);
void deinit_hidden_server(struct hidden_server *hs);
int add_stun(struct hidden_server *hs, uint8_t *addr, uint16_t port);
int hidden_send(struct hidden_server *hs, uint8_t data[12]);
int hidden_read(struct hidden_server *hs, uint8_t data[12]);

#endif
