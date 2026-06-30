#include <stdio.h>
#include <stdint.h>

#include "../../hidden/hidden.h" // include hidden header for using hidden server api

int main() {
    // init hidden server structure and stuns array
    struct hidden_server hs;
    init_hidden_server(&hs, 3);

    // add fake and real stun servers
    add_stun(&hs, "23.21.150.121", 3478);
    add_stun(&hs, "18.191.223.12", 3478);
    add_stun(&hs, "127.0.0.1", 3478);

    // send binding requests including data in transaction every stun servers
    uint8_t data[12] = "ping";
    hidden_send(&hs, data);

    // read every binding replies
    uint8_t reply[12];
    while (1) {
        if (hidden_read(&hs, reply) < 0) {
            continue; // real binding reply or no data
        }

        // print reply data
        printf("%s\n", (char *)reply);
    }
}
