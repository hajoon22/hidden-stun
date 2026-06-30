#include <stdio.h>
#include <stdint.h>

#include "../../hidden/hidden.h"

int main() {
    struct hidden_server hs;
    init_hidden_server(&hs, 3);

    add_stun(&hs, "23.21.150.121", 3478);
    add_stun(&hs, "18.191.223.12", 3478);
    add_stun(&hs, "127.0.0.1", 3478);

    uint8_t data[12] = "ping";
    hidden_send(&hs, data);

    uint8_t reply[12];
    while (1) {
        if (hidden_read(&hs, reply) < 0) {
            continue;
        }

        printf("%s\n", (char *)reply);
    }
}
