const MAXSTRING = 12;
const MAX_LENGTH = 15;

typedef string ip<MAX_LENGTH>;
typedef string article<MAXSTRING>;

struct trig {
int port;
ip addr;

};

struct pub {

article art;
ip addr;
int port;

};

struct sub {

ip addr;
int port;
article arti;

};

program COMMUNICATE_PROG {
	version COMMUNICATE_VERSION {

	bool JOIN(trig) = 1;
	bool LEAVE(trig) = 2;
	bool SUBSCRIBE(sub) = 3;
	bool UNSUBSCRIBE(sub) = 4;
	bool PUBLISH(pub) = 5;
	bool PING() = 6;

	} = 1;
	
} = 0x42424242;
