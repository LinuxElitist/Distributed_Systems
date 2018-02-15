CSCI 5105 : Distributed Systems : Project 1
Submitted By: Sandeep Badrinath Kowligi, Niharika Gupta, Mijia Jiang

To compile:
1. make rpc
2. make

How to use:

Launch the RPC server with ./serverside

Launch a client application in another process. Its usage is as follows;
./clientside “local ip” “local port” “server ip”

Options on client side (ping | join | leave | subscribe | unsubscribe | publish) where each has its function as follows -

1) Ping will return 0 if the server is up. This option is available to client irrespective of whether below mentioned operations are performed or not. 
2) Join will attempt to join the server. It uses your IP:Port as the unique identifier.
3) Subscribe will tell a server to make sure it pushes new and article-relevant content to your IP and specific port. Client can subscribe to content only after it has joined the server.
4) Unsubscribe will tell the server that the article is no more relevant to client. Client can unsubscribe from an article only if it is still joined to the server.
5) Publish of content is allowed to any client connected to the server. If the article has any subscribers, server makes sure to push content to interested clients. The max length of article that could be published is 120 bytes.
6) Leave will tell the server to eject your client if it was previously joined to the server.:
