#include "stdafx.h"
#include "Server.h"
#include "Player.h"

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
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <iostream>

#define BUFFLEN 1024
#define MAXCLIENTS 10


using namespace std;

int findemptyuser(IClient *clients[])
{
    int i;
    for (i = 0; i <  MAXCLIENTS; i++){
        if (clients[i]->GetSocket() == -1){
            return i;
        }
    }
    return -1;
}

Server::Server(unsigned int _port)
{
	port = _port;
}

int Server::CreateSocket()
{
	if ((l_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "ERROR #2: cannot create listening socket.\n");
		cin.get();
		return -1;
	}
}

void Server::SetServerAddress()
{
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
}

int Server::BindSocket()
{
	if (bind(l_socket, (struct sockaddr *)&servaddr, sizeof(servaddr))<0) {
		fprintf(stderr, "ERROR #3: bind listening socket.\n");
		cin.get();
		return -1;
	}
}

int Server::Listen()
{
	if (listen(l_socket, 5) <0) {
		fprintf(stderr, "ERROR #4: error in listen().\n");
		cin.get();
		return -1;
	}
}

void Server::GetConnections()
{
	socklen_t clientaddrlen;
	int maxfd = 0;

	FD_ZERO(&read_set);
	for (int i = 0; i < MAXCLIENTS; i++) {
		if (clients[i]->GetSocket() != -1) {
			FD_SET(clients[i]->GetSocket(), &read_set);
			if (clients[i]->GetSocket() > maxfd) {
				maxfd = clients[i]->GetSocket();
			}
		}
	}

	FD_SET(l_socket, &read_set);
	if (l_socket > maxfd) {
		maxfd = l_socket;
	}

	select(maxfd + 1, &read_set, NULL, NULL, NULL);

	if (FD_ISSET(l_socket, &read_set)) {
		int client_id = findemptyuser(clients);
		if (client_id != -1) {
			clientaddrlen = sizeof(clientaddr);
			memset(&clientaddr, 0, clientaddrlen);
			clients[client_id]->SetSocket(accept(l_socket, (struct sockaddr*)&clientaddr, &clientaddrlen));
			//c_sockets[client_id] = accept(l_socket, (struct sockaddr*)&clientaddr, &clientaddrlen);
			printf("Connected:  %s\n", inet_ntoa(clientaddr.sin_addr));
		}
	}
}

void Server::SendAndRecv()
{
	char buffer[BUFFLEN];

	for (int i = 0; i < MAXCLIENTS; i++) {
		if (clients[i]->GetSocket() != -1) {
			if (FD_ISSET(clients[i]->GetSocket(), &read_set)) {
				memset(&buffer, 0, BUFFLEN);
				int r_len = recv(clients[i]->GetSocket(), buffer, BUFFLEN, 0);
				printf("Received: %s", buffer);

				for (int j = 0; j < MAXCLIENTS; j++) {
					if (clients[j]->GetSocket() != -1) {
						int w_len = send(clients[j]->GetSocket(), buffer, r_len, 0);
						if (w_len <= 0) {
		#ifdef _WIN32
							closesocket(clients[j]->GetSocket());
		#else
							close(players[j]->GetSocket());
		#endif 
							clients[j]->SetSocket(-1);
						}
					}
				}
			}
		}
	}
}


void Server::Send(char buffer[])
{

	for (int i = 0; i < MAXCLIENTS; i++) {
		if (clients[i]->GetSocket() != -1) {
			if (FD_ISSET(clients[i]->GetSocket(), &read_set)) {
				for (int j = 0; j < MAXCLIENTS; j++) {
					if (clients[j]->GetSocket() != -1) {
						int w_len = send(clients[j]->GetSocket(), buffer, strlen(buffer), 0);
						if (w_len <= 0) {
#ifdef _WIN32
							closesocket(clients[j]->GetSocket());
#else
							close(players[j]->GetSocket());
#endif 
							clients[j]->SetSocket(-1);
						}
					}
				}
			}
		}
	}
}

void Server::Receive()
{
	char buffer[BUFFLEN];

	for (int i = 0; i < MAXCLIENTS; i++) {
		if (clients[i]->GetSocket() != -1) {
			if (FD_ISSET(clients[i]->GetSocket(), &read_set)) {
				memset(&buffer, 0, BUFFLEN);
				int r_len = recv(clients[i]->GetSocket(), buffer, BUFFLEN, 0);
				clients[i]->Act(buffer);
			}
		}
	}
	memset(&buffer, 0, BUFFLEN);
}



void Server::Init()
{
	DoStuff();
	CreateSocket();
	SetServerAddress();
	BindSocket();
	Listen();
}

int Server::DoStuff()
{
#ifdef _WIN32
	WSADATA data;
#endif
    
	

    if ((port < 1) || (port > 65535)){
        fprintf(stderr, "ERROR #1: invalid port specified.\n");
		cin.get();
        return -1;
    }

#ifdef _WIN32
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

    //1. CreateSocket()
	//2. SetServerAddress()
	//3. BindSocket()
	//4. Listen()
    //5. Start()

    return 0;
}