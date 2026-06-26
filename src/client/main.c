#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../stun/stun.h"

#define STUNS_LENGTH 6

struct stun {
    uint8_t *address;
    uint16_t port;
};

struct stun stuns[STUNS_LENGTH] = {
    {"23.21.150.121", 3478}, // real
    {"18.191.223.12", 3478}, // real
    {"85.17.88.164", 3478}, // real
    {"127.0.0.1", 3478}, // fake
    {"5.39.72.109", 3478}, // real
    {"216.93.246.18", 3478} // real
};

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) return s;

    uint8_t transaction[12] = "ping";
    for (int i = 0; i < STUNS_LENGTH; i++) {
        struct stun su = stuns[i];
        uint32_t stun_addr = ntohl(inet_addr(su.address));
        if (stun_addr == 0xFFFFFFFF) {
            continue;
        }

        send_binding_request(s, stun_addr, su.port, transaction);
    }

    uint32_t addr;
    uint16_t port;
    uint8_t reply[12];
    while (1) {
        int n = read_transaction(s, reply, &addr, &port);
        if (n < 0) continue;

        if (memcmp(transaction, reply, 12)) {
            printf("%s\n", (char *)reply);
        }
    }
}
