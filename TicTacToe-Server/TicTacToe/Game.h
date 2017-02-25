#pragma once

#include "Player.h"

#include <cstdlib>


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
	bool IsValidMove(int location);
};