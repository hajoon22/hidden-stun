#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h> 

static ssize_t build_binding_reply(uint8_t **buf, uint8_t transaction[12], uint32_t saddr, uint16_t sport) {
    *buf = malloc(32);
    if (!*buf) return -1;

    // binding reply header
    uint16_t type = htons(0x0101);
    uint16_t length = htons(12);
    uint32_t magic = htonl(0x2112A442);

    memcpy(*buf, &type, 2);
    memcpy(*buf+2, &length, 2);
    memcpy(*buf+4, &magic, 4);
    memcpy(*buf+8, transaction, 12);

    // x-mapped-address
    uint16_t attribute_type = htons(0x0020);
    uint16_t attribute_length = htons(0x0008);

    memcpy(*buf+20, &attribute_type, 2);
    memcpy(*buf+22, &attribute_length, 2);
    (*buf)[24] = 0x00; // Reserved
    (*buf)[25] = 0x01; // IPv4

    sport = htons(sport)^htons(0x2112); // xor
    saddr = htonl(saddr)^htonl(0x2112A442); // xor

    memcpy(*buf+26, &sport, 2);
    memcpy(*buf+28, &saddr, 4);
    
    return 32;
}

int send_binding_reply(int s, uint32_t addr, uint16_t port, uint8_t transaction[12]) {
    uint8_t *buf = NULL;
    ssize_t n = build_binding_reply(&buf, transaction, addr, port);
    if (n < 0) return -1; // failed build binding reply

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(addr);

    ssize_t r = sendto(s, buf, n, 0, (struct sockaddr *)&sin, sizeof(sin));
    free(buf);

    return r;
}

static ssize_t build_binding_request(uint8_t **buf, uint8_t transaction[12]) {
    *buf = malloc(20);
    if (!*buf) return -1; 
    
    uint16_t type = htons(0x0001);
    uint32_t magic = htonl(0x2112A442); 

    memcpy(*buf, &type, 2); // message type
    memset(*buf+2, 0, 2); // message length
    memcpy(*buf+4, &magic, 4); // magic cookie
    memcpy(*buf+8, transaction, 12); // transaction id (0)

    return 20;
}

int send_binding_request(int s, uint32_t addr, uint16_t port, uint8_t transaction[12]) {
    uint8_t *buf = NULL;
    ssize_t n = build_binding_request(&buf, transaction);
    if (n < 0) return -1; // failed build binding request

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(addr);

    ssize_t r = sendto(s, buf, n, 0, (struct sockaddr *)&sin, sizeof(sin));
    free(buf);

    return r;
}

int read_transaction(int s, uint8_t transaction[12], uint32_t *addr, uint16_t *port) {
    uint8_t *buf = malloc(20);
    if (!buf) return -1; // failed malloc

    struct sockaddr_in sin;
    socklen_t sin_len = sizeof(sin);
    ssize_t n = recvfrom(s, buf, 20, 0, (struct sockaddr *)&sin, &sin_len);
    if (n < 20) {
        free(buf);
        return -1; // recv error or invalid length
    }

    memcpy(transaction, buf+8, 12);

    *addr = ntohl(sin.sin_addr.s_addr);
    *port = ntohs(sin.sin_port);

    free(buf);
    return 0;
}
