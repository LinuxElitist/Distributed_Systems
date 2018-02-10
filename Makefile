CXX=g++
CXXFLAGS=-std=c++0x
LIBDIR=
LIBS=-lnsl  -lpthread

RPCGEN_FILE=communicate.x

SVC_GEN_SRCS=communicate_svc.cpp
CLI_GEN_SRCS=communicate_clnt.cpp
XDR_GEN_SRCS=communicate_xdr.cpp

CLI_SRCS=$(XDR_GEN_SRCS) $(CLI_GEN_SRCS) communicate_client.cpp
SVC_SRCS= $(XDR_GEN_SRCS) $(SVC_GEN_SRCS) communicate_server.cpp article.cpp \
    subscribe.cpp communication.cpp

CLIENT_APP=clientside
SERVER_APP=serverside

all: $(CLIENT_APP) $(SERVER_APP)

setup:
	rpcgen -N $(RPCGEN_FILE)
	rename 's/\.c/.cpp/' communicate_svc.c communicate_clnt.c communicate_xdr.c

$(CLIENT_APP): $(CLI_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(SERVER_APP): $(SVC_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(SVC_GEN_SRCS) $(CLI_GEN_SRCS) $(XDR_GEN_SRCS) $(CLIENT_APP) \
	    $(SERVER_APP) communicate.h
