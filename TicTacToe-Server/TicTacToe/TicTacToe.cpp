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

		int index = -1;

		for (int i = 0; i < MAXCLIENTS; i++) {
			if ((server->players[i]->GetSocket() != -1) && !(server->players[i]->HasOpponent()))
			{
				index = i;
				break;
			}
		}

		if (index != -1)
		{
			for (int i = 0; i < MAXCLIENTS; i++) {
				if ((server->players[i]->GetSocket() != -1) && (!server->players[i]->HasOpponent()) && (i != index))
				{
					server->players[index]->SetOpponent(server->players[i]);
					server->players[i]->SetOpponent(server->players[index]);

					Game *game = new Game();

					server->players[index]->SetGame(game);
					server->players[i]->SetGame(game);
					cout << "players have opponents now" << endl;
					break;
				}
			}
		}
		
		
		
		server->Receive();


		server->Send();

		

	}

	
	 
	

	return 0;
}
