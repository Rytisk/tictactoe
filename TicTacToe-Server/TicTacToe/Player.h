#pragma once

#include "Game.h"

#include <string>

class Game;

using namespace std;

class Player
{
private:
	Player * opponent;
	Game * game;
	int socket;
	bool hasOpponent;
	bool isWaiting;
public:
	string message;
	bool isInvalid;
	int GetSocket();
	void SetSocket(int socket);
	Player& GetOpponent();
	void SetOpponent(Player *player);
	Player();
	void Act(char message[]);
	void Wait(char message[]);
	bool HasOpponent();
	bool IsWaiting();
	void SetIsWaiting(bool value);
	void SetGame(Game *game);
};