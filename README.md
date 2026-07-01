## Hidden STUN
Hiding data inside the STUN transaction field.

## Overview
Hidden STUN sends Binding Requests to multiple STUN servers where most are real STUN servers and one is a fake STUN server controlled by this project. The fake STUN server returns a normal looking Binding Response, but hides data inside the transaction field. The client receives Binding Responses from all servers, checks the transaction field, and prints the hidden data when it is found.

## Workflow
1. Client sends Binding Requests to real STUN servers and the fake STUN server.
2. Real STUN servers return normal Binding Responses.
3. The fake STUN server returns a Binding Response with hidden data in the transaction field.
4. Client reads the transaction field from each response.
5. Client prints the hidden data.

## How to use another project
client:
```c
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
        break;
    }

    // deinit hidden server structure
    deinit_hidden_server(&hs); 
}
```

## How to Test
Build server and client:
```bash
make example
```

Run the server:
```bash
./server
```

Run the client:
```bash
./client
```
