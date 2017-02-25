// TicTacToe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "Game.h"
#include "Player.h"

#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
//#include <unistd.h>

using namespace std;

int main()
{
	Server *server = new Server(8001);
	server->Init();

	/*
	Game *game = new Game();

	Player *playerX = new Player();
	Player *playerO = new Player();

	playerX->SetOpponent(playerO);
	playerO->SetOpponent(playerX);

	game->SetCurrentPlayer(playerX);

	game->Begin(server);*/

	for (int i = 0; i < MAXCLIENTS; i++) {
		server->players[i] = new Player();
		server->players[i]->SetSocket(-1);
	}


	while(true)
	{

		server->GetConnections();

		//Kai klientas prisijungia nusiunciam jam:WAIT
		//ir laukiam kol pasijungs antras vartotojas, tada jiem abiem nustatysim setOpponent() viena su kitu
		//tada vienam nusius wait, kitam start

		Player *player = NULL;

		for (int i = 0; i < MAXCLIENTS; i++) {
			if ((server->players[i]->GetSocket() != -1) && !server->players[i]->HasOpponent())
			{
				player = server->players[i];
				break;
			}
		}

		for (int i = 0; i < MAXCLIENTS; i++) {
			if (player != NULL & (server->players[i]->GetSocket() != -1) && !server->players[i]->HasOpponent() && server->players[i] != player)
			{
				player->SetOpponent(server->players[i]);
				server->players[i]->SetOpponent(player);
				
				Game *game = new Game();

				player->SetGame(game);
				server->players[i]->SetGame(game);
				cout << "players have opponents now" << endl;
				break;
			}
		}
		
		player = NULL;
		//server->SendAndRecv();

		server->Receive();


	//	for (int i = 0; i < MAXCLIENTS; i++) {
		//	if (server->players[i]->GetSocket() != -1 && server->players[i]->HasOpponent())
		//	{
				
		server->Send("a");
				//server->SendWithAddress(&server->players[i]->GetOpponent(), a);
			
			
	//	}

		

	}

	
	 
	

	return 0;
}

/*
int main(int argc, char *argv[]) {
#ifdef _WIN32
	WSADATA data;
#endif
	unsigned int port;
	int l_socket; // socket'as skirtas prisijungimø laukimui
	int c_socket; // prisijungusio kliento socket'as

	struct sockaddr_in servaddr; // Serverio adreso struktûra
	struct sockaddr_in clientaddr; // Prisijungusio kliento adreso struktûra
								   //    int clientaddrlen;
	socklen_t clientaddrlen;

	int s_len;
	int r_len;
	char buffer[1024];

	argv[1] = "8001";

//	if (argc != 2) {
//		printf("USAGE: %s <port>\n", argv[0]);
//		exit(1);
//	}

	port = atoi(argv[1]);
	cout << port << endl;
	

	if ((port < 1) || (port > 65535)) {
		printf("ERROR #1: invalid port specified.\n");
		exit(1);
	}

#ifdef _WIN32
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	
	// Sukuriamas serverio socket'as
	
	if ((l_socket = socket(AF_INET, SOCK_STREAM, 0))< 0) {
		fprintf(stderr, "ERROR #2: cannot create listening socket.\n");
		exit(1);
	}

	
	// Iðvaloma ir uþpildoma serverio adreso struktûra
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; // nurodomas protokolas (IP)

								   
								   // Nurodomas IP adresas, kuriuo bus laukiama klientø, ðiuo atveju visi
								   // esami sistemos IP adresai (visi interfeis'ai)
								  
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	cout << inet_ntoa(servaddr.sin_addr) << endl;

	servaddr.sin_port = htons(port); // nurodomas portas

									 
									 // Serverio adresas susiejamas su socket'u
									 
	if (bind(l_socket, (struct sockaddr *)&servaddr, sizeof(servaddr))<0) {
		fprintf(stderr, "ERROR #3: bind listening socket.\n");
		exit(1);
	}

	
	//Nurodoma, kad socket'u l_socket bus laukiama klientø prisijungimo,
	//eilëje ne daugiau kaip 5 aptarnavimo laukiantys klientai
	
	if (listen(l_socket, 5) <0) {
		fprintf(stderr, "ERROR #4: error in listen().\n");
		exit(1);
	}

	for (;;) {
		
		//Iðvalomas buferis ir kliento adreso struktûra
		
		memset(&clientaddr, 0, sizeof(clientaddr));
		memset(&buffer, 0, sizeof(buffer));


		cout << "Laukiam prisijungimo"  << endl;
		
		//Laukiama klientø prisijungimø
		
		clientaddrlen = sizeof(struct sockaddr);
		if ((c_socket = accept(l_socket,
			(struct sockaddr*)&clientaddr, &clientaddrlen))<0) {
			fprintf(stderr, "ERROR #5: error occured accepting connection.\n");
			exit(1);
		}
		cout << "Prisijunge" << endl;
		
		// Sulaukus kliento prisijungimo skaitomi kliento siunèiami duomenys
		
		s_len = recv(c_socket, buffer, sizeof(buffer), 0);

		
		// Klientui iðsiunèiami gauti duomenys
		

		r_len = send(c_socket, buffer, s_len, 0);


	//	printf("IP: %s Sent: %d Received: %d\n", inet_ntoa(clientaddr.sin_addr),
	//		s_len, r_len
	//		);

		
		// Klientas atjungiamas
		
#ifdef _WIN32
		closesocket(c_socket);
#else
		close(c_socket);
#endif 
	}

	return 0;
}
*/