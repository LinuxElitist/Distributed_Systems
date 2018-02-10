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
#define PORT 5105
#define BUFSIZE 512

using namespace std;

class Client {

public:
  CLIENT *clnt;
  int pingtoserver = 5; //To be changed maybe
  bool continueListeningThread = false;
  void join (char *ip , int port);
  void leave (char *ip , int port);
  void subscribe (char *ip , int port , char *art);
  void unsubscribe (char *ip , int port , char *art);
  void publish (char *art , char *ip , int port);
  void ping(void);
  std::thread t1;
  Client(char *ip , int port) {
    clnt = clnt_create(SERV_IP,COMMUNICATE_PROG,COMMUNICATE_VERSION, "udp");
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
    char buf[BUFSIZE];
    socklen_t length = sizeof(message);

    if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)  {
      perror("socket creation failed");
      exit(1);
    }
    memset((char *) &message, 0, sizeof(message));
    message.sin_family = AF_INET;
    message.sin_port =htons(PORT); // Port must be defined
    message.sin_addr.s_addr = inet_addr("134.84.121,102"); //Server must be sent here //TODO

    memset(buf,'\0', BUFSIZE); //TODO define buf and BUFSIZE

    if ( recvfrom (sockfd,buf,strlen(buf),0,(struct sockaddr *)&message, &length) < 0 ) {

      perror("Did not get response from server");

    }
    puts(buf);

  }  
};

void Listen(Client *c)
{
  // Listen for UDP messages from the server
  while (c->continueListeningThread){
    printf("Listening...");
    c->listen_for_article();
  }
}

void Client::join (char *ip , int port) {
  auto output = join_1(ip,port,clnt);
  if (output == NULL) {
    clnt_perror(clnt, "Join failed for server");
  }
  continueListeningThread = true;
  t1 = std::thread(Listen,this);
  
}

void Client::leave (char *ip , int port) {
  auto output = leave_1(ip,port,clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Leave failed");
  }
}

void Client::subscribe (char *ip , int port , char *art)
{
  auto output = subscribe_1(ip,port,art,clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Subscribe failed");
  }
}

void Client::unsubscribe (char *ip , int port ,char *art)
{
  auto output = unsubscribe_1(ip,port,art,clnt);
  if (output == NULL) {
	  clnt_perror(clnt, "Unsubscribe failed");
  }
}

void Client::publish (char *art , char *ip , int port)
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

// void listen_for_article() {

//   struct sockaddr_in message;

//   int sockfd;
//   char buf[BUFSIZE];
//   socklen_t length = sizeof(message);
  
//   if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)  {
//     perror("socket creation failed");
//     exit(1);
//   }
//   memset((char *) &message, 0, sizeof(message));
//   message.sin_family = AF_INET;
//   message.sin_port =htons(PORT); // Port must be defined
//   message.sin_addr.s_addr = inet_addr("134.84.121,102"); //Server must be sent here //TODO

//   memset(buf,'\0', BUFSIZE); //TODO define buf and BUFSIZE

//   if ( recvfrom (sockfd,buf,strlen(buf),0,(struct sockaddr *)&message, &length) < 0 ) {

//     perror("Did not get response from server");

//   }
//   puts(buf); 

// }

int main(int argc, char* argv[]) {

  if (argc < 3) {
    std::cout << "Usage: ./clientside client_ip client_port\n" ;
    exit(1);
  }
  int *output;
  char *client_ip = (char*)argv[1];
  int client_port = stoi(argv[2]);
  int func_number;
  char article_string[1024];
  //  std::thread t1(listen_for_article);
  //t1.join();
  
  Client conn(client_ip, client_port);

  while(1){
    std::cout << "Please enter what function you want to perform [1-6]:\n" << "Function description\n1 Ping\n2 Join\n3 Subscribe\n4 Unsubscribe\n5 Publish\n6 Leave\n" ;
    std::cin >> func_number;
    if((func_number==3)||(func_number==4)||(func_number==5)){
      std::cout <<"Please enter the article for above function:\n" << "\"Type;Originator;Org;Contents\"\n"
                << "\tType can be <Sports, Lifestyle, Entertainment, Business, Technology, Science, Politics, Health>\n" ;
      std::cin >> article_string;
    }

    switch(func_number){
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
        std::cout << "Wrong format specified. Please retry \n" ;
        break;
    }
  }
}
