#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../../hidden/stun/stun.h"

#define LISTEN_PORT 3478

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) return s;

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0; // 0.0.0.0
    sin.sin_port = htons(LISTEN_PORT);

    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        close(s);
        return -1;
    }
    printf("stun listening on 0.0.0.0:%d\n", LISTEN_PORT);
    
    uint32_t addr;
    uint16_t port;
    uint8_t transaction[12];
    while (1) {
        int n = read_transaction(s, transaction, &addr, &port);
        if (n < 0) continue;

        printf("%s\n", (char *)transaction);

        uint8_t reply[12] = "pong";
        send_binding_reply(s, addr, port, reply);
    }
}
