// TicTacToe-Client.cpp : Defines the entry point for the console application.
//

#include "Client.h"
#include "stdafx.h"

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
#include <string>
#include <iostream>


using namespace std;

Client *client;

BOOL ctrl_handler(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT) {
		client->Exit();
		return true;
	}
	return false;
}

int main(int argc, char *argv[]) 
{

	unsigned int port;
	string ip;

	client = new Client();
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);

	

	cout << "Enter port number: " << endl;
	cin >> port;

	cout << "Enter IP address: " << endl;
	cin >> ip;

	cin.get();

	client->Init();

	if (client->CreateSocket() < 0)
		exit(1);
	if (client->SetAddress(port, ip) < 0)
		exit(1);
	if (client->Connect() < 0)
		exit(1);

	client->Run();
	return 0;
}