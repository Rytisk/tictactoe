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
#include <string.h>
#include <iostream>


#define BUFFLEN 1024
using namespace std;

int Client::DoStuff()
{
#ifdef _WIN32
	WSADATA data;
#endif    
	unsigned int port;
	int s_socket;
	struct sockaddr_in servaddr; // Serverio adreso struktûra
	fd_set read_set;
	char ipAddress[] = "172.16.12.210";
	char recvbuffer[BUFFLEN];
	char sendbuffer[BUFFLEN];
	char buffer[BUFFLEN];

	int i;
	/*
	if (argc != 3) {
		fprintf(stderr, "USAGE: %s <ip> <port>\n", argv[0]);
		exit(1);
	}*/

	port = 8001;

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
	servaddr.sin_addr.s_addr = inet_addr(ipAddress);
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
	memset(&sendbuffer, 0, BUFFLEN);
	//fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | O_NONBLOCK);
/*	while (1) {
		FD_ZERO(&read_set);
		FD_SET(s_socket, &read_set);
		FD_SET(0, &read_set);

		select(s_socket + 1, &read_set, NULL, NULL, NULL);

		if (FD_ISSET(s_socket, &read_set)) {
			memset(&recvbuffer, 0, BUFFLEN);
			i = read(s_socket, &recvbuffer, BUFFLEN);
			printf("%s\n", recvbuffer);
		}
		else if (FD_ISSET(0, &read_set)) {
			i = read(0, &sendbuffer, 1);
			write(s_socket, sendbuffer, i);
		}
	}*/

	    printf("Enter the message: ");
	    fgets(buffer, BUFFLEN, stdin);
	/*
	* Iðsiunèiamas praneðimas serveriui
	*/
	    send(s_socket,buffer,strlen(buffer),0);

	    memset(&buffer,0,BUFFLEN);
	/*
	* Praneðimas gaunamas ið serverio
	*/
	    recv(s_socket,buffer,BUFFLEN,0);
	    printf("Server sent: %s\n", buffer);

	/*
	* Socket'as uþdaromas
	*/
#ifdef _WIN32
	closesocket(s_socket);
#else
	close(s_socket);
#endif 
	return 0;
}