#include "communication.h"
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#define MAX_ARTICLE_LENGTH 120

using std::to_string;

int send_client(const Subscriber &sub, const char *buf) {
    int result = -1;
    int bytes_sent = 0;
    char padding[MAX_ARTICLE_LENGTH];
    char padded[MAX_ARTICLE_LENGTH];
    memset((void*)padding,'\0', (MAX_ARTICLE_LENGTH - strlen(buf)));
    strcpy(padded,buf);
    strcat(padded,padding);

    std::cout << "padded buf " << padded << "\n";

    const char *ip = sub.ip.c_str();
    const char *port = (to_string(sub.port)).c_str();
    //const int port = sub.port;
    struct addrinfo sendaddr;
    struct addrinfo *res = 0;

    memset(&sendaddr, 0, sizeof(sendaddr));
    sendaddr.ai_family = AF_UNSPEC;
    sendaddr.ai_socktype = SOCK_DGRAM;
    sendaddr.ai_protocol = 0;
    sendaddr.ai_flags = AI_ADDRCONFIG;

    if (getaddrinfo(ip, port, &sendaddr, &res) != 0) {
        return result;
    }

    std::cout << "padded buf " << padded << "\n";

//TODO: Not sure how binding is happening
    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == -1) {
        std::cout << "could not create udp socket on server side \n";
        freeaddrinfo(res);
        close(fd);
        return result;
    }

    bytes_sent = sendto(fd, padded, MAX_ARTICLE_LENGTH, 0, res->ai_addr, res->ai_addrlen);
    if (bytes_sent < 0) {
        std::cout << "bytes sent: " << bytes_sent << "\n";
        freeaddrinfo(res);
        close(fd);
        return result;
    }

    std::cout << "bytes sent: " << bytes_sent << "\n";

    freeaddrinfo(res);
    close(fd);
	return 0;
}
