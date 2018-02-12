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
        struct sockaddr_in message;
        int sockfd;
        char buf[MAX_ARTICLE_LENGTH];
        int bytes_received = 0;

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
            perror("socket creation failed");
            exit(1);
        }
    		int optval = 1;
    		setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *) &optval, sizeof(int));


        memset((char *) &message, 0, sizeof(message));
        message.sin_family = AF_INET;
        message.sin_port = htons(this->udp_port); // Port must be defined
        //message.sin_addr.s_addr = inet_addr(INADDR_LOOPBACK); //Server must be sent here //TODO
        if (inet_aton(SERV_IP , &message.sin_addr) == 0) {
  	        fprintf(stderr, "inet_aton() failed\n");
  	        exit(1);
  	    }
        socklen_t length = sizeof(message);

	      memset(buf,'\0',MAX_ARTICLE_LENGTH);

        bytes_received = recvfrom(sockfd, buf, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &message, &length);
	      //if( recvfrom(sockfd, buf, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &message, &length) == -1 ) {
	      if ( bytes_received < 0) {
            perror("Did not get response from server");
	          close(sockfd);
            exit(1);
        }
        puts(buf);
        std::cout << bytes_received << " bytes received for " << buf << "\n";
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
    if (*output < 0) {
        clnt_perror(clnt, "Join failed for server");
    }
}

void Client::leave(char *ip, int rpc_port) {
    auto output = leave_1(ip, rpc_port, clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Leave failed");
    }
}

void Client::subscribe(char *ip, int rpc_port, char *art) {
    auto output = subscribe_1(ip, rpc_port, art, clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Subscribe failed");
    }
}

void Client::unsubscribe(char *ip, int rpc_port, char *art) {
    auto output = unsubscribe_1(ip, rpc_port, art, clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Unsubscribe failed");
    }
}

void Client::publish(char *art, char *ip, int rpc_port) {
    auto output = publish_1(art, ip, rpc_port, clnt);
    continueListeningThread = true;
    t1 = std::thread(Listen, this);
    if (*output < 0) {
        clnt_perror(clnt, "Publish failed");
    }
}

void Client::ping() {
    auto output = ping_1(clnt);
    if (*output < 0) {
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
