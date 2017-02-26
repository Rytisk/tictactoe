#pragma once
#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#define MAXCLIENTS 10
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include "stdafx.h"

class Server {
private:
	unsigned int port;
	int l_socket;
	struct sockaddr_in servaddr;
	struct sockaddr_in clientaddr;
	int c_sockets[MAXCLIENTS];
	
	fd_set read_set;
public:
	Player *players[MAXCLIENTS];
	Server(unsigned int _port);
	int DoStuff();
	int CreateSocket();
	void SetServerAddress();
	int BindSocket();
	int Listen();
	void Send();
	void Receive();
	void Init();
	void GetConnections();
	void SendWithAddress(Player *player, char buffer[]);
	//void SendAndRecv();
};