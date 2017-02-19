#include "stdafx.h"
#include "Server.h"

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

int findemptyuser(int c_sockets[])
{
    int i;
    for (i = 0; i <  MAXCLIENTS; i++){
        if (c_sockets[i] == -1){
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

void Server::Start()
{
	socklen_t clientaddrlen;

	int c_sockets[MAXCLIENTS];
	fd_set read_set;

	int maxfd = 0;
	int i;

	char buffer[BUFFLEN];
	
	for (i = 0; i < MAXCLIENTS; i++) {
		c_sockets[i] = -1;
	}

	for (;;) {
		FD_ZERO(&read_set);
		for (i = 0; i < MAXCLIENTS; i++) {
			if (c_sockets[i] != -1) {
				FD_SET(c_sockets[i], &read_set);
				if (c_sockets[i] > maxfd) {
					maxfd = c_sockets[i];
				}
			}
		}

		FD_SET(l_socket, &read_set);
		if (l_socket > maxfd) {
			maxfd = l_socket;
		}

		select(maxfd + 1, &read_set, NULL, NULL, NULL);

		if (FD_ISSET(l_socket, &read_set)) {
			int client_id = findemptyuser(c_sockets);
			if (client_id != -1) {
				clientaddrlen = sizeof(clientaddr);
				memset(&clientaddr, 0, clientaddrlen);
				c_sockets[client_id] = accept(l_socket, (struct sockaddr*)&clientaddr, &clientaddrlen);
				printf("Connected:  %s\n", inet_ntoa(clientaddr.sin_addr));
			}
		}
		for (i = 0; i < MAXCLIENTS; i++) {
			if (c_sockets[i] != -1) {
				if (FD_ISSET(c_sockets[i], &read_set)) {
					memset(&buffer, 0, BUFFLEN);
					int r_len = recv(c_sockets[i], buffer, BUFFLEN, 0);

					int j;
					for (j = 0; j < MAXCLIENTS; j++) {
						if (c_sockets[j] != -1) {
							int w_len = send(c_sockets[j], buffer, r_len, 0);
							if (w_len <= 0) {
#ifdef _WIN32
								closesocket(c_sockets[j]);
#else
								close(c_sockets[j]);
#endif 
								c_sockets[j] = -1;
							}
						}
					}
				}
			}
		}
	}
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