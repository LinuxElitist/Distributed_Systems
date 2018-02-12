#include "communication.h"
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#define SERV_IP "127.0.0.1"

#define MAX_ARTICLE_LENGTH 120
#define UDP_PORT 1234

using std::to_string;

int send_client(const Subscriber &sub, const char *buf) {

    int fd; /* socket */
    //int port; /* UDP_PORTto listen on */
    socklen_t clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char hello[6]; /* message buf */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval = 1;
    int result = -1;
    int bytes_sent = 0;
    char padding[MAX_ARTICLE_LENGTH];
    char padded[MAX_ARTICLE_LENGTH];
    bzero(padding, (MAX_ARTICLE_LENGTH - strlen(buf)));
    strcpy(padded,buf);
    strcat(padded,padding);

    std::cout << "padded buf " << padded << "\n";

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) {
        std::cout << "could not create udp socket on server side \n";
        close(fd);
        return result;
    }

		setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(const void *) &optval, sizeof(int));

  //  const char *ip = sub.ip.c_str();
  //  const char *UDP_PORT= (to_string(sub.port)).c_str();

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port= htons((unsigned short)UDP_PORT);
/*    if (inet_aton(SERV_IP, &serveraddr.sin_addr)==0) {
        std::cout << "ERROR on inet_aton\n";
        close(fd);
        return result;
    }
*/
    if (bind(fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
        std::cout << "could not bind \n";
        close(fd);
        return result;
    }

    clientlen = sizeof(clientaddr);

    bzero(hello, 6);
    int bytes_received = recvfrom(fd, hello, 6, 0, (struct sockaddr *) &clientaddr, &clientlen);
    if (bytes_received < 0){
        std::cout << "could not receive from client\n";
        close(fd);
        return result;
    }

    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL) {
        std::cout << "ERROR on gethostbyaddr\n";
        close(fd);
        return result;
    }
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL) {
        std::cout << "ERROR on inet_ntoa\n";
        close(fd);
        return result;
    }
    std::cout << "server received datagram from" << hostp->h_name << hostaddrp << "\n";


//TODO..............  put ip from subscribed list            .............


    bytes_sent = sendto(fd, padded, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &clientaddr, clientlen);
    if (bytes_sent < 0) {
        std::cout << "could not send to client\n";
        close(fd);
        return result;
    }

    close(fd);
    result = 0;
    return result;
}
