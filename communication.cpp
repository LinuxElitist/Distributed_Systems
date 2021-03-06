#include "communication.h"
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>

using std::to_string;

int send_client(const Subscriber &sub, const char *buf) {
    const char *ip = sub.ip.c_str();
    const char *port = (to_string(sub.port)).c_str();

    struct addrinfo sendaddr;
    struct addrinfo *res = 0;

    memset(&sendaddr, 0, sizeof(sendaddr));
    sendaddr.ai_family = AF_UNSPEC;
    sendaddr.ai_socktype = SOCK_DGRAM;
    sendaddr.ai_protocol = 0;
    sendaddr.ai_flags = AI_ADDRCONFIG;

    if (getaddrinfo(ip, port, &sendaddr, &res) != 0) {
        return 5; // 5 => send_client failed
    }

    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == -1) {
        freeaddrinfo(res);
        return 5; // 5 => send_client failed
    }

    if (sendto(fd, buf, strlen(buf), 0, res->ai_addr, res->ai_addrlen) == -1) {
        freeaddrinfo(res);
        close(fd);
        return 5; // 5 => send_client failed
    }
    freeaddrinfo(res);
    close(fd);
    return 0;
}
