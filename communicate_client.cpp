#include <iostream>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "communicate.h"
#include "article.h"
#include <netinet/in.h>
#include <netdb.h>
#define SERV_IP "127.0.0.1"
#define MAX_ARTICLE_LENGTH 120

using namespace std;

class Client {

public:
    CLIENT *clnt;
    int pingtoserver = 5; //To be changed maybe
    bool continueListeningThread = false;
    int udp_port;

    void join(char *ip, int rpc_port);

    void leave(char *ip, int rpc_port);

    void subscribe(char *ip, int rpc_port, char *art);

    void unsubscribe(char *ip, int rpc_port, char *art);

    void publish(char *art, char *ip, int rpc_port);

    void ping(void);

    std::thread t1;

    Client(char *ip, int rpc_port) {
        clnt = clnt_create(SERV_IP, COMMUNICATE_PROG, COMMUNICATE_VERSION, "udp");
        if (clnt == NULL) {
            clnt_pcreateerror(SERV_IP);
            exit(1);
        }
    }

    ~Client() {
        clnt_destroy(clnt);
        t1.join();
    }

    void listen_for_article() {
        struct sockaddr_in serveraddr;
        int sockfd;
        struct hostent *server;
        char buf[MAX_ARTICLE_LENGTH];
        int bytes_received = 0;
        socklen_t serverlen;
        int optval = 1;
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            std::cout << "socket creation failed\n";
            close(sockfd);
            exit(1);
        }
        setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *) &optval, sizeof(int));

        /* gethostbyname: get the server's DNS entry */
        server = gethostbyname(SERV_IP);
        if (server == NULL) {
            std::cout << "ERROR, no such host as " << SERV_IP << "\n";
            close(sockfd);
            exit(1);
        }

        /* build the server's Internet address */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
        (char *)&serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(this->udp_port);

        serverlen = sizeof(serveraddr);

        int bytes_sent = sendto(sockfd, "hello", 6, 0, (struct sockaddr *)&serveraddr, serverlen);
        if (bytes_sent < 0){
            std::cout << "Could not connect to server\n";
            close(sockfd);
            exit(1);
        }

        bzero(buf, MAX_ARTICLE_LENGTH);

        bytes_received = recvfrom(sockfd, buf, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &serveraddr, &serverlen);
	      if ( bytes_received < 0) {
            std::cout << "Did not get response from server\n";
	          close(sockfd);
            exit(1);
        }
        std::cout << bytes_received << " bytes received for " << buf << "\n";
        close(sockfd);
        continueListeningThread = false;
    }
};

void Listen(Client *c) {
    // Listen for UDP messages from the server
    while (c->continueListeningThread) {
        std::cout << "Listening...\n";
        c->listen_for_article();
    }
}

void Client::join(char *ip, int rpc_port) {
    auto output = join_1(ip, rpc_port, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Join failed for server");
    }
}

void Client::leave(char *ip, int rpc_port) {
    auto output = leave_1(ip, rpc_port, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Leave failed");
    }
}

void Client::subscribe(char *ip, int rpc_port, char *art) {
    auto output = subscribe_1(ip, rpc_port, art, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Subscribe failed");
    }
}

void Client::unsubscribe(char *ip, int rpc_port, char *art) {
    auto output = unsubscribe_1(ip, rpc_port, art, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Unsubscribe failed");
    }
}

void Client::publish(char *art, char *ip, int rpc_port) {
    continueListeningThread = true;
    t1 = std::thread(Listen, this);
    auto output = publish_1(art, ip, rpc_port, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Publish failed");
    }
}

void Client::ping() {
    auto output = ping_1(clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Cannot ping server");
    }
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        std::cout << "Usage: ./clientside client_ip rpc_port udp_port\n";
        exit(1);
    }
    char *client_ip = (char *) argv[1];
    int client_rpc_port = stoi(argv[2]);
    char func[1];
    int func_number;
    char article_string[MAX_ARTICLE_LENGTH];

    Client conn(client_ip, client_rpc_port);
    conn.udp_port = stoi(argv[3]);
    //std::thread t1(Listen,&conn);
    //t1.join();
    while (1) {
        std::cout << "Please enter what function you want to perform [1-6]:\n"
                  << "Function description\n1 Ping\n2 Join\n3 Subscribe\n4 Unsubscribe\n5 Publish\n6 Leave\n";
        std::cin >> func;
        func_number = stoi(func);
        if ((func_number == 3) || (func_number == 4) || (func_number == 5)) {
            std::cout << "Please enter the article for above function:\n" << "\"Type;Originator;Org;Contents\"\n"
                      << "\tType can be <Sports, Lifestyle, Entertainment, Business, Technology, Science, Politics, Health>\n";
            std::cin >> article_string;
        }

        switch (func_number) {
            case 1:
                conn.ping();
                break;
            case 2:
                conn.join(client_ip, client_rpc_port);
                break;
            case 3:
                conn.subscribe(client_ip, client_rpc_port, article_string);
                break;
            case 4:
                conn.unsubscribe(client_ip, client_rpc_port, article_string);
                break;
            case 5:
                conn.publish(article_string, client_ip, client_rpc_port);
                break;
            case 6:
                conn.leave(client_ip, client_rpc_port);
                break;
            default:
                std::cout << "Wrong format specified. Please retry \n";
                break;
        }
    }
}
