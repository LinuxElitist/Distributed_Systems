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
#define SERV_IP "128.101.37.18"
#define MAX_ARTICLE_LENGTH 120

using namespace std;

class Client {

public:
    CLIENT *clnt;
    int pingtoserver = 5; //To be changed maybe
    bool continueListeningThread = false;
    int sock;
    char article[MAX_ARTICLE_LENGTH];
    void join(char *ip, int port);

    void leave(char *ip, int port);

    void subscribe(char *ip, int port, char *art);

    void unsubscribe(char *ip, int port, char *art);

    void publish(char *art, char *ip, int port);

    void ping(void);

    std::thread t1;

    Client(char *ip, int port) {

        clnt = clnt_create(SERV_IP, COMMUNICATE_PROG, COMMUNICATE_VERSION, "udp");
        if (clnt == NULL) {
            clnt_pcreateerror(SERV_IP);
            exit(1);
        }
        //join(ip,port);
         printf("Completed client creation\n");
    }

    ~Client() {

        t1.join();
        clnt_destroy(clnt);
    }

    void listen_for_article(int port) {

            // Waits to receive an article message
      struct sockaddr_in si_other, client_addr;
      int i;
      socklen_t slen=sizeof(si_other);
      int optval=1;

      if ( (sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
      {
          perror("socket");
          exit(1);
      }
      setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const void *) &optval, sizeof(int));

      memset((char *) &si_other, 0, sizeof(si_other));
      si_other.sin_family = AF_INET;
      si_other.sin_port = htons(port);

      if (inet_aton(SERV_IP , &si_other.sin_addr) == 0)
      {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
      }

      bzero((char *) &client_addr, sizeof(client_addr));
      client_addr.sin_family = AF_INET;
      client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      client_addr.sin_port= htons((unsigned short)port);
      if (bind(sock, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0){
          std::cout << "could not bind \n";
          close(sock);
          exit(1);
      }


      // Clear the buffer by filling null, it might have previously received data
      memset(article,'\0', MAX_ARTICLE_LENGTH);

      // Try to receive some data; This is a blocking call
      //TODO : add padding
      if (recvfrom(sock, article, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &si_other, &slen) == -1)
      {
          perror("recvfrom()");
          exit(1);
      }
      //puts(article);
      std::cout << "content received: " << article << "\n";
    }
};

void Listen(Client *c, int port) {
    // Listen for UDP messages from the server
    while (c->continueListeningThread) {
        std::cout << "Listening...\n";
        c->listen_for_article(port);
    }
}

void Client::join(char *ip, int port) {
    auto output = join_1(ip, port, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Join failed for server");
    }
    continueListeningThread = true;
     t1 = std::thread(Listen, this, port);

  // printf("%s:%d has successfully joined the server.\n", IP, Port);
}

void Client::leave(char *ip, int port) {
    auto output = leave_1(ip, port, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Leave failed");
    }
}

void Client::subscribe(char *ip, int port, char *art) {
    auto output = subscribe_1(ip, port, art, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Subscribe failed");
    }
}

void Client::unsubscribe(char *ip, int port, char *art) {
    auto output = unsubscribe_1(ip, port, art, clnt);
    if ((output == NULL)|| (*output < 0)) {
        clnt_perror(clnt, "Unsubscribe failed");
    }
}

void Client::publish(char *art, char *ip, int port) {
    //continueListeningThread = true;
   // t1 = std::thread(Listen, this);
    auto output = publish_1(art, ip, port, clnt);
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

    if (argc < 3) {
        std::cout << "Usage: ./clientside client_ip port udp_port\n";
        exit(1);
    }
    char *client_ip = (char *) argv[1];
    int client_port = stoi(argv[2]);
    char func[1];
    int func_number;
    char article_string[MAX_ARTICLE_LENGTH];

    Client conn(client_ip, client_port);
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
                conn.join(client_ip, client_port);
                break;
            case 3:
                conn.subscribe(client_ip, client_port, article_string);
                break;
            case 4:
                conn.unsubscribe(client_ip, client_port, article_string);
                break;
            case 5:
                conn.publish(article_string, client_ip, client_port);
                break;
            case 6:
                conn.leave(client_ip, client_port);
                break;
            default:
                std::cout << "Wrong format specified. Please retry \n";
                break;
        }
    }
}
