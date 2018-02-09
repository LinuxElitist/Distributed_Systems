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

class client {

public:
  CLIENT *clnt;
  int pingtoserver = 5; //To be changed maybe

  clnt = clnt_create(SERV_IP,);
  
  
  
  

}
  int main() {

    fprintf(stderr, "number of args: %d\n",argc);
    

    if (argc < 3) {

      fprintf(stderr,"check usage:client ip:client port");
    }

    client conn();
}
