// TicTacToe-Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
//#include <unistd.h>


#define BUFFLEN 1024

using namespace std;

int s_socket;

string trim_right(const std::string& str)
{
	const std::string pattern = " \f\n\r\t\v";
	return str.substr(0, str.find_last_not_of(pattern) + 1);
}

void PrintBoard(char board[])
{
	cout << board[0] << "  |  " << board[1] << "  |  " << board[2] << endl;
	cout << "_____________" << endl;
	cout << board[3] << "  |  " << board[4] << "  |  " << board[5] << endl;
	cout << "_____________" << endl;
	cout << board[6] << "  |  " << board[7] << "  |  " << board[8] << endl;

}

void CloseConnection(int s_socket)
{
	#ifdef _WIN32
		closesocket(s_socket);
	#else
		close(s_socket);
	#endif 
}

BOOL ctrl_handler(DWORD event)
{
	string qu = "QUIT";
	if (event == CTRL_CLOSE_EVENT) {
		send(s_socket, &qu[0u], strlen(&qu[0u]), 0);
		return true;
	}
	return false;
}


int main(int argc, char *argv[]) {

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);

	char board[9] = { '0', '1', '2',
					  '3', '4', '5',
					  '6', '7', '8' };

#ifdef _WIN32
	WSADATA data;
#endif    
	unsigned int port;
	
	struct sockaddr_in servaddr; // Serverio adreso struktûra

	char buffer[BUFFLEN];

	//if (argc != 3) {
	//	fprintf(stderr, "USAGE: %s <ip> <port>\n", argv[0]);
	//	exit(1);
	//}
	argv[2] = "8001";
	argv[1] = "127.0.0.1";

	port = atoi(argv[2]);

	if ((port < 1) || (port > 65535)) {
		printf("ERROR #1: invalid port specified.\n");
		exit(1);
	}

#ifdef _WIN32
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	/*
	* Sukuriamas socket'as
	*/
	if ((s_socket = socket(AF_INET, SOCK_STREAM, 0))< 0) {
		fprintf(stderr, "ERROR #2: cannot create socket.\n");
		exit(1);
	}

	/*
	* Iðvaloma ir uþpildoma serverio struktûra
	*/
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; // nurodomas protokolas (IP)
	servaddr.sin_port = htons(port); // nurodomas portas

									 /*
									 * Iðverèiamas simboliø eilutëje uþraðytas ip á skaitinæ formà ir
									 * nustatomas serverio adreso struktûroje.
									 */
#ifdef _WIN32
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
#else
	if (inet_aton(argv[1], &servaddr.sin_addr) <= 0) {
		fprintf(stderr, "ERROR #3: Invalid remote IP address.\n");
		exit(1);
	}
#endif        


	/*
	* Prisijungiama prie serverio
	*/
	if (connect(s_socket, (struct sockaddr*)&servaddr, sizeof(servaddr))<0) {
		fprintf(stderr, "ERROR #4: error in connect().\n");
		exit(1);
	}


	string str;

	str = "CONN";
	
	send(s_socket, &str[0u], strlen(&str[0u]), 0);

	string ex = "EXIT";

	string qu = "QUIT";
	

	
	while (true) 
	{
		PrintBoard(board);
		cout << endl;
		

		memset(&buffer, 0, BUFFLEN);
		recv(s_socket, buffer, BUFFLEN, 0);
		printf("Server sent: %s\n", buffer);

		str = buffer;
		str = trim_right(str);

		if (str == "WAIT")		//Jei kitas pradeda, tu klausai, ka jis padarys
		{
			memset(&buffer, 0, BUFFLEN);
			recv(s_socket, buffer, BUFFLEN, 0);
			printf("Server sent: %s\n", buffer);
			str = buffer;
			str = trim_right(str);
		}

		
		
		if (str == "WIN")
		{
			cout << "You won" << endl;
			send(s_socket, &ex[0u], strlen(&ex[0u]), 0);
			break;
		}
		else if (str == "LOSE")
		{
			cout << "You lost" << endl;
			send(s_socket, &ex[0u], strlen(&ex[0u]), 0);
			break;
		}
		else if (str == "TIE")
		{
			cout << "It's a tie" << endl;
			send(s_socket, &ex[0u], strlen(&ex[0u]), 0);
			break;
		}
		
		if (str == "QUIT")
		{
			cout << "Opponent has quit" << endl;
			send(s_socket, &qu[0u], strlen(&qu[0u]), 0);
			break;
		}
		
		if (str.substr(0,4) == "MOVE")
		{

			board[stoi(str.substr(4,5))] = 'O';
			PrintBoard(board);
			cout << endl;
		}

		
		memset(&buffer, 0, BUFFLEN);
		printf("Enter the message: ");
		fgets(buffer, BUFFLEN, stdin);
		/*
		* Iðsiunèiamas praneðimas serveriui
		*/
		

		
		

		str = buffer;
		str = trim_right(str);
		
		if (str == "QUIT")
		{
			send(s_socket, &qu[0u], strlen(&qu[0u]), 0);
			break;
		}
		
		send(s_socket, buffer, strlen(buffer), 0);

		int location = stoi(str.substr(4,5));

		memset(&buffer, 0, BUFFLEN);
		recv(s_socket, buffer, BUFFLEN, 0);

		
		board[location] = 'X';
		printf("Server sent: %s\n", buffer);
		cout << "||" << str << "||" << endl;

		


		memset(&buffer, 0, BUFFLEN);
		/*
		* Praneðimas gaunamas ið serverio
		*/

	}
	cin.get();
	CloseConnection(s_socket);
	return 0;
}