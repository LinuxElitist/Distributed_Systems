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
#define UDP_PORT 1234
#define SERV_IP "127.0.0.1"

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

    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        std::cout << "could not create udp socket on server side \n";
        close(fd);
        return result;
    }
    int optval = 1;
		setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(const void *) &optval, sizeof(int));

    const char *ip = sub.ip.c_str();
    const char *port = (to_string(sub.port)).c_str();
    struct addrinfo clnt_addr, server_addr;
    struct addrinfo *res = 0;
    struct addrinfo *res_serv = 0;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.ai_family = AF_INET;
    server_addr.ai_socktype = SOCK_DGRAM;
    server_addr.ai_protocol = 0;
    server_addr.ai_flags = AI_ADDRCONFIG;

    if (getaddrinfo(SERV_IP, (to_string(UDP_PORT)).c_str(), &server_addr, &res_serv) != 0) {
        std::cout << "could not get addrinfo \n";
        freeaddrinfo(res);
        freeaddrinfo(res_serv);
        close(fd);
        return result;
    }

    memset(&clnt_addr, 0, sizeof(clnt_addr));
    clnt_addr.ai_socktype = SOCK_DGRAM;
    clnt_addr.ai_family = AF_INET;
    clnt_addr.ai_protocol = 0;
    clnt_addr.ai_flags = AI_ADDRCONFIG;

    if (getaddrinfo(ip, port, &clnt_addr, &res) != 0) {
        std::cout << "could not get addrinfo \n";
        freeaddrinfo(res);
        freeaddrinfo(res_serv);
        close(fd);
        return result;
    }

    if(bind(fd, res_serv->ai_addr, res_serv->ai_addrlen) < 0){
        std::cout << "could not bind server\n";
        freeaddrinfo(res);
        freeaddrinfo(res_serv);
        close(fd);
        return result;
    }

    bytes_sent = sendto(fd, padded, MAX_ARTICLE_LENGTH, 0, res->ai_addr, res->ai_addrlen);
    if (bytes_sent < 0) {
        std::cout << "bytes sent: " << bytes_sent << "\n";
        freeaddrinfo(res);
        freeaddrinfo(res_serv);
        close(fd);
        return result;
    }

    std::cout << "bytes sent: " << bytes_sent << "\n";

    freeaddrinfo(res);
    freeaddrinfo(res_serv);
    close(fd);
	return 0;
}
