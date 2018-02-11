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
<<<<<<< HEAD
=======

>>>>>>> 57dead3a7d26f3f9d9481fe0809d2b9218baebce
#define SERV_IP "127.0.0.1"
#define CLNT_IP "127.0.0.1"
#define SERV_PORT 5105
#define CLNT_PORT 5105
#define MAX_ARTICLE_LENGTH 120

using namespace std;

class Client {

public:
    CLIENT *clnt;
    int pingtoserver = 5; //To be changed maybe
    bool continueListeningThread = false;

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
	//	int optval = 1;
	//	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *) &optval, sizeof(int));
	
        memset((char *) &message, 0, sizeof(message));
        message.sin_family = AF_INET;
        message.sin_port = htons(SERV_PORT); // Port must be defined
        //message.sin_addr.s_addr = inet_addr(SERV_IP); //Server must be sent here //TODO
	if (inet_aton(SERV_IP , &message.sin_addr) == 0)
	  {
	    fprintf(stderr, "inet_aton() failed\n");
	    exit(1);
	  }
	socklen_t length = sizeof(message);

	memset(buf,'\0',MAX_ARTICLE_LENGTH);
	
	if ( recvfrom(sockfd, buf, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &message, &length) == -1 ) {
	// if ( bytes_received < 0) {
            perror("Did not get response from server");
	    //            close(sockfd);
            exit(1);
        }

  puts(buf);
  //     std::cout << bytes_received << " bytes received for " << buf << "\n";

    }

    /*void listen_for_article() {

        char buf[MAX_ARTICLE_LENGTH];
        int bytes_received = 0;
        socklen_t length = sizeof(struct sockaddr_in);

        struct sockaddr_in srvr_addr, clnt_addr;
        int fd;

        if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1) {
            std::cout << "server udp socket creation failed\n";
            close(fd);
            exit(1);
        }
        int optval = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        memset((char *) &srvr_addr, 0, sizeof(srvr_addr));
        srvr_addr.sin_family = AF_INET;
        srvr_addr.sin_port = htons(SERV_PORT);
        //srvr_addr.sin_addr.s_addr = inet_addr(SERV_IP);
        if (inet_aton("128.101.37.120", &srvr_addr.sin_addr)==0) {
            fprintf(stderr, "inet_aton() failed for srvr addr\n");
            close(fd);
            exit(1);
        }

		clnt_addr.sin_family = AF_INET;
		clnt_addr.sin_port = htons(CLNT_PORT);
		//clnt_addr.sin_addr.s_addr = inet_addr(CLNT_IP);

		if (inet_aton("128.101.37.47", &clnt_addr.sin_addr)==0) {
			fprintf(stderr, "inet_aton() failed for clnt addr\n");
			close(fd);
			exit(1);
	    }
		std::cout << "server: " << inet_ntoa(srvr_addr.sin_addr) << ":" << srvr_addr.sin_port << "\n";
		std::cout << "client: " << inet_ntoa(clnt_addr.sin_addr) << ":" << clnt_addr.sin_port << "\n";

		if (bind(fd, (struct sockaddr *) &clnt_addr, sizeof(clnt_addr))==-1){
			std::cout << "bind client udp socket failed\n";
			close(fd);
			exit(1);
	    }


        //memset(buf, '\0', MAX_ARTICLE_LENGTH);

        //recvfrom might not work because we dont know the exact number of bytes transferred.
        //to make it work, pad article to MAX_ARTICLE_LENGTH bytes on server side
        bytes_received = recvfrom(fd, buf, MAX_ARTICLE_LENGTH, 0, (struct sockaddr *) &clnt_addr, &length);
        if ( bytes_received < 0) {
            perror("Did not get response from server");
            close(fd);
            exit(1);
        }
        std::cout << bytes_received << " bytes received for " << buf << "\n";
        //puts(buf);
    }*/
};

void Listen(Client *c) {
    // Listen for UDP messages from the server
    while (c->continueListeningThread) {
        std::cout << "Listening...\n";
        c->listen_for_article();
    }
}

void Client::join(char *ip, int port) {
    auto output = join_1(ip, port, clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Join failed for server");
    }
    //    continueListeningThread = true;
    // t1 = std::thread(Listen, this);

}

void Client::leave(char *ip, int port) {
    auto output = leave_1(ip, port, clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Leave failed");
    }
}

void Client::subscribe(char *ip, int port, char *art) {
    auto output = subscribe_1(ip, port, art, clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Subscribe failed");
    }
}

void Client::unsubscribe(char *ip, int port, char *art) {
    auto output = unsubscribe_1(ip, port, art, clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Unsubscribe failed");
    }
}

void Client::publish(char *art, char *ip, int port) {
    auto output = publish_1(art, ip, port, clnt);
    //    continueListeningThread = true;
    //t1 = std::thread(Listen, this)
    if (*output < 0) {
        clnt_perror(clnt, "Publish failed");
    }
    continueListeningThread = true;
    t1 = std::thread(Listen, this);
    //listen_for_article();
}

void Client::ping() {
    auto output = ping_1(clnt);
    if (*output < 0) {
        clnt_perror(clnt, "Cannot ping server");
    }
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cout << "Usage: ./clientside client_ip client_port\n";
        exit(1);
    }
    char *client_ip = (char *) argv[1];
    int client_port = stoi(argv[2]);
    char func[1];
    int func_number;
    char article_string[MAX_ARTICLE_LENGTH];

    Client conn(client_ip, client_port);
<<<<<<< HEAD
    std::thread t1(Listen,&conn);
    //    t1.join();
=======
    //std::thread t1(Listen,&conn);
    //t1.join();
>>>>>>> 57dead3a7d26f3f9d9481fe0809d2b9218baebce

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
