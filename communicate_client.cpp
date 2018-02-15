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

#define MAX_ARTICLE_LENGTH 120

using namespace std;

class Client;
void Listen(Client *c, int port);

class Client {

public:
    CLIENT *clnt;
    int PingServer = 5; //time interval to ping group server
    bool listeningthread = false;

    int sock; //socket descriptor
    char article[MAX_ARTICLE_LENGTH]; //received article
    const char error_msg[7][40] = {"Success", "Client should first be joined", "Client should first be subscribed",
                                   "Wrong Format of article for operation", "Wrong Type of article",
                                   "UDP communication from server failed", "Operation Failed"};

    void join(char *ip, int port);

    void leave(char *ip, int port);

    void subscribe(char *ip, int port, char *art);

    void unsubscribe(char *ip, int port, char *art);

    void publish(char *art, char *ip, int port);

    int ping(void);

    std::thread udp_thread; //thread for udp unicast

    char *serv_ip;

    Client(char *ip, int port, char *serv) {
        serv_ip = serv;
        clnt = clnt_create(serv_ip, COMMUNICATE_PROG, COMMUNICATE_VERSION, "udp");
        if (clnt == NULL) {
            clnt_pcreateerror(serv_ip);
            exit(1);
        }
        std::cout << ".....Completed client creation.....\n";
    }

    ~Client() {
      if (clnt){
	clnt_destroy(clnt); }
       
    }

    /*Function implementation to receive udp communication*/

    void listen_for_article(int port) {
        struct sockaddr_in si_other, client_addr;
        int i;
        socklen_t slen = sizeof(si_other);
        int optval = 1;

        if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
            perror("socket()");
            exit(1);
        }
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int));

        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(port);

        if (inet_aton(serv_ip, &si_other.sin_addr) == 0) {
            fprintf(stderr, "inet_aton failed\n");
            exit(1);
        }

        bzero((char *) &client_addr, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        client_addr.sin_port = htons((unsigned short) port);
        if (bind(sock, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
            fprintf(stderr, "could not bind \n");
            close(sock);
            exit(1);
        }

	listeningthread = true;
	udp_thread = std::thread(Listen, this, port);
	udp_thread.detach();
	// Clear the buffer by filling null, it might have previously received data

        memset(article, '\0', MAX_ARTICLE_LENGTH);

        // Try to receive some data; This is a blocking call
        if (recvfrom(sock, article, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &si_other, &slen) == -1) {
            perror("recvfrom()");
            exit(1);
        }
        Article art(article);
        std::cout << "..... \"" << art.content << "\" RECEIVED for \"" << art.type << ";" << art.orig << ";" << art.org << "\" .....\n";
    }

};

void Listen(Client *c, int port) {
    // Listen for UDP messages from the server
    while (c->listeningthread) {
        std::cout << "Listening......\n";
        c->listen_for_article(port);
    }
}


void Client::join(char *ip, int port) {
    auto output = join_1(ip, port, clnt);
    if (output == NULL) {
        clnt_perror(clnt, "Join failed for server");
    } else {
        std::cout << "..... " << error_msg[*output] << " .....\n";
    }
    
}

void Client::leave(char *ip, int port) {
    auto output = leave_1(ip, port, clnt);
    if (output == NULL) {
        clnt_perror(clnt, "Leave failed");
    } else {
        std::cout << "..... " << error_msg[*output] << " .....\n";
    }
}

void Client::subscribe(char *ip, int port, char *art) {
    auto output = subscribe_1(ip, port, art, clnt);
    if (output == NULL) {
        clnt_perror(clnt, "Subscribe failed");
    } else {
        std::cout << "..... " << error_msg[*output] << " .....\n";
    }
}

void Client::unsubscribe(char *ip, int port, char *art) {
    auto output = unsubscribe_1(ip, port, art, clnt);
    if (output == NULL) {
        clnt_perror(clnt, "Unsubscribe failed");
    } else {
        std::cout << "..... " << error_msg[*output] << " .....\n";
    }
}

void Client::publish(char *art, char *ip, int port) {
    auto output = publish_1(art, ip, port, clnt);
    if (output == NULL) {
        clnt_perror(clnt, "Publish failed");
    } else {
        std::cout << "..... " << error_msg[*output] << " .....\n";
    }
}

int Client::ping() {
    auto output = ping_1(clnt);
    if (output == NULL) {
        clnt_perror(clnt, "Cannot ping server");
        return 1;
    } else {
        std::cout << "..... " << error_msg[*output] << " .....\n";
    }
    return *output;
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        std::cout << "Usage: ./clientside client_ip port server_ip\n";
        exit(1);
    }
    char *client_ip = (char *) argv[1];
    int client_port = stoi(argv[2]);
    char *serv_ip = (char *) argv[3];
    char func[1];
    int func_number;
    char article_string[MAX_ARTICLE_LENGTH];

    Client conn(client_ip, client_port, serv_ip);

    while (1) {
        std::cout << "Please enter what function you want to perform [1-6]:\n"
                  << "Function description\n1 Ping\n2 Join\n3 Subscribe\n4 Unsubscribe\n5 Publish\n6 Leave\n";
        std::cin >> func;
        try {
            func_number = stoi(func);
        }
        catch(std::exception& e) {
            cout << "ERROR:  Please limit operation values from 1-6 " << endl;
            continue;
        }
        switch (func_number) {
            case 1:
                conn.ping();
                break;
            case 2:
                conn.join(client_ip, client_port);
                break;
            case 3:
                std::cout << "Please enter the genre of article to subscribe to:\n\"Type;Originator;Org\" One of these fields is a MUST\n"
                          << " Type can be <Sports, Lifestyle, Entertainment, Business, Technology, Science, Politics, Health>\n";
                std::cin.get(); // ignore the \n from before article is entered
                std::cin.getline(article_string, MAX_ARTICLE_LENGTH);
                conn.subscribe(client_ip, client_port, article_string);
                break;
            case 4:
                std::cout << "Please enter the genre of article to unsubscribe from:\n\"Type;Originator;Org\" One of these fields is a MUST\n"
                          << " Type can be <Sports, Lifestyle, Entertainment, Business, Technology, Science, Politics, Health>\n";
                std::cin.get(); // ignore the \n from before article is entered
                std::cin.getline(article_string, MAX_ARTICLE_LENGTH);
                conn.unsubscribe(client_ip, client_port, article_string);
                break;
            case 5:
                std::cout << "Please enter the genre & content of article to publish to:\n\"Type;Originator;Org;Content\" One of these genres is a MUST along with contents\n"
                          << " Type can be <Sports, Lifestyle, Entertainment, Business, Technology, Science, Politics, Health>\n"
                          << "Please limit to 120 bytes for article\n";
                std::cin.get(); // ignore the \n from before article is entered
                std::cin.getline(article_string, MAX_ARTICLE_LENGTH);
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
