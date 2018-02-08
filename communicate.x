const MAXSTRING = 120;
const MAX_LENGTH = 15;

typedef string ip<MAX_LENGTH>;
typedef string article<MAXSTRING>;

program COMMUNICATE_PROG {
	version COMMUNICATE_VERSION {
	    bool JOIN(ip IP, int Port) = 1;
	    bool LEAVE(ip IP, int Port) = 2 ;
	    bool SUBSCRIBE(ip IP, int Port, article Article) =3;
	    bool UNSUBSCRIBE(ip IP, int Port, article Article)=4;
	    bool PUBLISH(article Article, ip IP, int Port)=5;
	    bool PING() = 6;
        } = 1;
} = 0x42424242;
