#ifndef STUN_H
#define STUN_H

#include <stdint.h>

int read_transaction(int s, uint8_t transaction[12], uint32_t *addr, uint16_t *port);
int send_binding_reply(int s, uint32_t addr, uint16_t port, uint8_t transaction[12]);
int send_binding_request(int s, uint32_t addr, uint16_t port, uint8_t transaction[12]);

#endif
