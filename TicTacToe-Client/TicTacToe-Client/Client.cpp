#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"

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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>


#define BUFFLEN 1024
using namespace std;



void Client::Exit()
{
	string qu = "QUIT";
	send(s_socket, &qu[0u], strlen(&qu[0u]), 0);
}

string trim_right(const std::string& str)
{
	const std::string pattern = " \f\n\r\t\v";
	return str.substr(0, str.find_last_not_of(pattern) + 1);
}

void Client::PrintBoard(char board[])
{
	cout << board[0] << "  |  " << board[1] << "  |  " << board[2] << endl;
	cout << "_____________" << endl;
	cout << board[3] << "  |  " << board[4] << "  |  " << board[5] << endl;
	cout << "_____________" << endl;
	cout << board[6] << "  |  " << board[7] << "  |  " << board[8] << endl;

}

void Client::CloseConnection(int s_socket)
{
#ifdef _WIN32
	closesocket(s_socket);
#else
	close(s_socket);
#endif 
}


int Client::Connect()
{
	if (connect(s_socket, (struct sockaddr*)&servaddr, sizeof(servaddr))<0) {
		fprintf(stderr, "ERROR #4: error in connect().\n");
		return -1;
	}
	return 1;
}

int Client::CreateSocket()
{
	if ((s_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "ERROR #2: cannot create socket.\n");
		return -1;
	}
	return 1;
}

int Client::SetAddress(unsigned int port, string ip)
{
	if ((port < 1) || (port > 65535)) {
		printf("ERROR #1: invalid port specified.\n");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);


#ifdef _WIN32
	servaddr.sin_addr.s_addr = inet_addr(&ip[0u]);
#else
	if (inet_aton(&ip[0u], &servaddr.sin_addr) <= 0) {
		fprintf(stderr, "ERROR #3: Invalid remote IP address.\n");
		return -1;
	}
#endif 
	return 1;
}

void Client::Init()
{
	#ifdef _WIN32
		WSADATA data;
	#endif 

	#ifdef _WIN32
		WSAStartup(MAKEWORD(2, 2), &data);
	#endif
}

void Client::Run()
{
	bool quit = false;
	char board[9] = { '0', '1', '2',
					  '3', '4', '5',
					  '6', '7', '8' };

	
	char buffer[BUFFLEN];

	string message = "CONN";
	string ex = "EXIT";
	string qu = "QUIT";

	send(s_socket, &message[0u], strlen(&message[0u]), 0);



	while (true)
	{
		PrintBoard(board);
		cout << endl;

		memset(&buffer, 0, BUFFLEN);
		recv(s_socket, buffer, BUFFLEN, 0);

		message = buffer;
		message = trim_right(message);


		if (message == "WIN" || message == "LOSE" || message == "TIE")
		{
			cout << message << endl;
			send(s_socket, &ex[0u], strlen(&ex[0u]), 0);
			break;
		}
		else if (message == "QUIT")
		{
			cout << "Opponent has quit" << endl;
			send(s_socket, &qu[0u], strlen(&qu[0u]), 0);
			break;
		}
		else if (message.substr(0, 4) == "EXIT")
		{
			cout << "Server disconnected" << endl;
			break;
		}
		else if (message.substr(0, 4) == "MOVE")
		{
			board[stoi(message.substr(4, 5))] = 'O';
			PrintBoard(board);
			cout << endl;
		}


		while (true)
		{
			memset(&buffer, 0, BUFFLEN);
			printf("Enter the message: ");
			fgets(buffer, BUFFLEN, stdin);


			message = buffer;
			message = trim_right(message);
			char num = message.at(0);
			if (message == "QUIT")
			{
				send(s_socket, &qu[0u], strlen(&qu[0u]), 0);
				quit = true;
				break;
			}
			else if (num <= '9' && num >= '0' && (board[num-'0'] != 'X' && board[num-'0'] != 'O'))
			{
				int w_len = send(s_socket, &num, strlen(&num), 0);
				int location = num - '0';
				memset(&buffer, 0, BUFFLEN);
				board[location] = 'X';
				if (w_len < 0)
				{
					quit = true;
				}
				break;
			}
			else
			{
				cout << "Invalid input" << endl;
			}
		}

		if (quit)
		{
			break;
		}
	}
	cout << "Press any key to exit..." << endl;
	cin.get();
	CloseConnection(s_socket);
}