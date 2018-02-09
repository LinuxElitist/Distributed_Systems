#include <iostream>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "communicate.h"

/*yet to do this */

#define SERV_IP
#define PORT 5105
#define BUFLEN 512

using namespace std;

class Client {

public:
  CLIENT *clnt;
  int pingtoserver = 5; //To be changed maybe

  clnt = clnt_create(SERV_IP,COMMUNICATE_PROG,COMMUNICATE_VERSION, "udp" );
  if (clnt == NULL)
    {
      clnt_pcreateerror(SERV_IP);
      exit(1);
    }

  //to be implemented
  ~Client() {
    




  };
  /*Calling all the server function here */

  void join (char *ip , int port) {

    output = join_1(ip,port,clnt);
    if (output == NULL)
      {
	clnt_perror(clnt, "Join failed for server");
      }

  }


  void leave (char *ip , int port) {

    output = leave_1(ip,port,clnt);
    if (output == NULL)
      {
	clnt_perror(clnt, "Leave failed");
      }

  }

  void subscribe (char *ip , int port ,Article art) //Check struct
  {

    output = subscribe_1(ip,port,art,clnt);
    if (output == NULL)
      {
	clnt_perror(clnt, "Subscribe failed");
      }

  }

  void unsubscribe (char *ip , int port ,Article art) //Check struct
  {

    output = unsubscribe_1(ip,port,art,clnt);
    if (output == NULL)
      {
	clnt_perror(clnt, "Unsubscrbe failed");
      }

  }

  void publish (Article art , char *ip , int port) //Check struct
  {

    output = publish_1(art,ip,port,clnt);
    if (output == NULL)
      {
	clnt_perror(clnt, "Publish failed");
      }

  }
  
  void ping() {
    output = ping_1(clnt);
    if (output == NULL)
      {
	clnt_perror(clnt, "Cannot ping server");
      }
  } 

  //  #ifndef DEBUG
  clnt_destroy(clnt);
  // #endif
  

  void listen_for_article() {
    struct sockaddr_in message;
    



  }

  
  int main() {

    
    fprintf(stderr, "number of args: %d\n",argc);
    

    if (argc < 3) {
      fprintf(stderr,"check usage:client ip:client port");
    }

    int *output;
    char *client_ip = (char*)argv[1];
    /* Dont know if this port assignment will work*/
    int port = argv[2];

    Client conn(client_ip,client_port);
}
