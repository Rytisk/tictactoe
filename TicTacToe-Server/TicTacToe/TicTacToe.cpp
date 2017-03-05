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

using namespace std;

Server *server;

BOOL ctrl_handler(DWORD event)
{

	if (event == CTRL_CLOSE_EVENT) {
		for (int i = 0; i < MAXCLIENTS; i++) {
			if (server->players[i] != NULL)
			{
				server->players[i]->message = "EXIT";
			}
		}
		server->Send();
		return true;
	}
	return false;
}

void SetOpponents()
{
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

				cout << "Players have opponents now" << endl;
				break;
			}
		}
	}
}

int main()
{
	server = new Server(8001);
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);

	server->Init();

	while(true)
	{
		server->GetConnections();

		SetOpponents();

		server->SendAndRecv();
	}

	return 0;
}
