#pragma once
#include "IClient.h"

class Player: public IClient
{
private:
	Player * opponent;
	int socket;
public:
	int GetSocket();
	void SetSocket(int _socket);
	Player& GetOpponent();
	void SetOpponent(Player *player);
	Player();
	void Act(char message[]);
};