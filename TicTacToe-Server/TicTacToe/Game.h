#pragma once
#include "Player.h"
#include "Server.h"
#include <cstdlib>

using namespace std;

class Game {
private:
	Player *board[9] = { NULL, NULL, NULL, 
						 NULL, NULL, NULL,
					     NULL, NULL, NULL };
	Player *currentPlayer;
public:
	void SetCurrentPlayer(Player *player);
	Game();
	bool HasWon();
	bool BoardIsFull();
	bool MakeAMove(Player *player, int location);
	Player &GetCurrentPlayer();
	void Begin(Server *server);
};