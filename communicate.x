program  COMMUNICATE_PROG {
    version COMMUNICATE_VERSION {
        /* Client IP, port */
        int JOIN(string, int) = 1;
        /* Client IP, port */
        int LEAVE(string, int) = 2;
        /* Client IP, port, article */
        int SUBSCRIBE(string, int, string) = 3;
        /* Client IP, port, article */
        int UNSUBSCRIBE(string, int, string) = 4;
        /* article, Client IP, port */
        int PUBLISH(string, string, int) = 5;
        /* IP, port */
        int PING() = 6;
    } = 1;
} = 0x20000001;
