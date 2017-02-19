#pragma once
#include "Player.h"
#include <cstdlib>

using namespace std;

class Game {
private:
	Player *board[9] = { NULL, NULL, NULL, 
						 NULL, NULL, NULL,
					     NULL, NULL, NULL };
	Player *currentPlayer;
public:
	Game(Player *player);
	bool HasWon();
	bool BoardIsFull();
	bool MakeAMove(Player *player, int location);
};