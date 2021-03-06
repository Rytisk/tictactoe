#include "stdafx.h"
#include "Game.h"
#include <iostream>

Game::Game() {
	
}

bool Game::HasWon() {
	return (board[0] != 0 && board[0] == board[1] && board[0] == board[2]) ||
		(board[0] != 0 && board[0] == board[3] && board[0] == board[6]) ||
		(board[0] != 0 && board[0] == board[4] && board[0] == board[8]) ||
		(board[1] != 0 && board[1] == board[4] && board[1] == board[7]) ||
		(board[2] != 0 && board[2] == board[5] && board[2] == board[8]) ||
		(board[2] != 0 && board[2] == board[4] && board[2] == board[6]) ||
		(board[3] != 0 && board[3] == board[4] && board[3] == board[5]) ||
		(board[6] != 0 && board[6] == board[7] && board[6] == board[8]);
}

bool Game::BoardIsFull() {
	for (int i = 0; i < 9; i++) {
		if (board[i] == NULL)
			return false;
	}
	return true;
}

bool Game::MakeAMove(Player *player, int location) {
	if (board[location] == NULL) {
		board[location] = player;
		return true;
	}
	return false;
}

void Game::SetCurrentPlayer(Player *player)
{
	currentPlayer = player;
}

Player& Game::GetCurrentPlayer()
{
	return *currentPlayer;
}

bool Game::IsValidMove(int location)
{
	if (location <= 8 && location >= 0 && board[location] == 0)
		return true;

	return false;
}