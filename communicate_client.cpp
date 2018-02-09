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
/*yet to do this */

#define SERV_IP "127.0.0.1"
#define PORT 5105
#define BUFLEN 512

using namespace std;

class Client {

public:
  CLIENT *clnt;
  //char *host = "127.0.0.1";
  int pingtoserver = 5; //To be changed maybe
  void join (char *ip , int port);
  void leave (char *ip , int port);
  void subscribe (char *ip , int port , Article art);
  void unsubscribe (char *ip , int port , Article art);
  void publish (Article art , char *ip , int port);
  void ping(void);

  Client(char *ip , int port) {
    clnt = clnt_create(SERV_IP,COMMUNICATE_PROG,COMMUNICATE_VERSION, "udp");
    if (clnt == NULL)
      {
        clnt_pcreateerror(SERV_IP);
        exit(1);
      }
    //to be implemented
  }
  ~Client() {
    clnt_destroy(clnt);
  }
};


/*Calling all the server function here */

void Client::join (char *ip , int port) {
  auto output = join_1(ip,port,clnt);
  if (output == NULL) {
    clnt_perror(clnt, "Join failed for server");
  }
}

void Client::leave (char *ip , int port) {
  auto output = leave_1(ip,port,clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Leave failed");
  }
}

void Client::subscribe (char *ip , int port , Article art) //Check struct
{
  auto output = subscribe_1(ip,port,art,clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Subscribe failed");
  }
}

void Client::unsubscribe (char *ip , int port ,Article art) //Check struct
{
  auto output = unsubscribe_1(ip,port,art,clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Unsubscrbe failed");
  }
}

void Client::publish (Article art , char *ip , int port) //Check struct
{
  auto output = publish_1(art,ip,port,clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Publish failed");
  }
}

void Client::ping() {
  auto output = ping_1(clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Cannot ping server");
  }
}

void listen_for_article() {
  struct sockaddr_in message;
}

int main(int argc, char* argv[]) {

  fprintf(stderr, "number of args: %d\n",argc);
  if (argc < 3) {
    fprintf(stderr,"check usage:client ip:client port");
  }
  int *output;
  char *client_ip = (char*)argv[1];
  int client_port = *argv[2];
  Client conn(client_ip, client_port);
}
