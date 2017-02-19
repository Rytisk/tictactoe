#pragma once
#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class Server {
private:
	unsigned int port;
	int l_socket;
	struct sockaddr_in servaddr;
	struct sockaddr_in clientaddr;
public:
	Server(unsigned int _port);
	int DoStuff();
	int CreateSocket();
	void SetServerAddress();
	int BindSocket();
	int Listen();
	void Start();
};