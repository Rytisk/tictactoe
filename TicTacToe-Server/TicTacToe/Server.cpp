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

#include <string>
#include <iostream>

#define BUFFLEN 1024
#define MAXCLIENTS 10


using namespace std;

int Server::FindEmptyUser()
{
    int i;
    for (i = 0; i <  MAXCLIENTS; i++)
	{
        if (players[i]->GetSocket() == -1)
		{
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
	if ((l_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		fprintf(stderr, "ERROR #2: cannot create listening socket.\n");
		cin.get();
		return -1;
	}
	return 1;
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
	if (bind(l_socket, (struct sockaddr *)&servaddr, sizeof(servaddr))<0) 
	{
		fprintf(stderr, "ERROR #3: bind listening socket.\n");
		cin.get();
		return -1;
	}
	return 1;
}

int Server::Listen()
{
	if (listen(l_socket, 5) <0) 
	{
		fprintf(stderr, "ERROR #4: error in listen().\n");
		return -1;
	}
	return 1;
}

void Server::GetConnections()
{
	socklen_t clientaddrlen;
	int maxfd = 0;

	FD_ZERO(&read_set);
	for (int i = 0; i < MAXCLIENTS; i++)
	{
		if (players[i]->GetSocket() != -1) 
		{
			FD_SET(players[i]->GetSocket(), &read_set);
			if (players[i]->GetSocket() > maxfd) 
			{
				maxfd = players[i]->GetSocket();
			}
		}
	}

	FD_SET(l_socket, &read_set);
	if (l_socket > maxfd) 
	{
		maxfd = l_socket;
	}

	select(maxfd + 1, &read_set, NULL, NULL, NULL);

	if (FD_ISSET(l_socket, &read_set)) 
	{
		int client_id = FindEmptyUser();
		if (client_id != -1) 
		{
			clientaddrlen = sizeof(clientaddr);
			memset(&clientaddr, 0, clientaddrlen);
			players[client_id]->SetSocket(accept(l_socket, (struct sockaddr*)&clientaddr, &clientaddrlen));
			printf("Connected:  %s\n", inet_ntoa(clientaddr.sin_addr));
		}
	}
}

void Server::SendAndRecv()
{
	char buffer[BUFFLEN];

	for (int i = 0; i < MAXCLIENTS; i++) 
	{
		if (players[i]->GetSocket() != -1) 
		{
			if (FD_ISSET(players[i]->GetSocket(), &read_set))
			{
				memset(&buffer, 0, BUFFLEN);
				int r_len = recv(players[i]->GetSocket(), buffer, BUFFLEN, 0);
				

				if (!players[i]->HasOpponent())
				{
					players[i]->Wait(buffer);
				}

				if (players[i]->IsWaiting())
				{
					CloseSocket(i);
				}

				else if (players[i]->HasOpponent() && !players[i]->isInvalid)
				{
					players[i]->Act(buffer);
					int w_len;
					
					w_len = send((&players[i]->GetOpponent())->GetSocket(), &(players[i]->message)[0u], strlen(&(players[i]->message)[0u]), 0);

					if (w_len <= 0) 
					{
						CloseSocket(i);
					}
				}
			}
		}
	}
}

void Server::CloseSocket(int index)
{
#ifdef _WIN32
	closesocket(players[index]->GetSocket());
#else
	close(players[i]->GetSocket());
#endif 

	players[index] = new Player();
	players[index]->SetSocket(-1);
}


void Server::Send()
{
	for (int i = 0; i < MAXCLIENTS; i++) 
	{
		if (players[i]->GetSocket() != -1) 
		{
			if (FD_ISSET(players[i]->GetSocket(), &read_set)) 
			{

				int w_len = send(players[i]->GetSocket(), &(players[i]->message)[0u], strlen(&(players[i]->message)[0u]), 0);
				if (w_len <= 0) 
				{
					CloseSocket(i);
				}

			}
		}
	}
}

int Server::Init()
{
#ifdef _WIN32
	WSADATA data;
#endif


	if ((port < 1) || (port > 65535)) 
	{
		fprintf(stderr, "ERROR #1: invalid port specified.\n");
		cin.get();
		return -1;
	}

#ifdef _WIN32
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	CreateSocket();
	SetServerAddress();
	BindSocket();
	Listen();

	for (int i = 0; i < MAXCLIENTS; i++) {
		players[i] = new Player();
		players[i]->SetSocket(-1);
	}

	return 1;
}