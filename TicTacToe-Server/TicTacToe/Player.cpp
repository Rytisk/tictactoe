#include "stdafx.h"
#include "Player.h"

#include <iostream>
#include <string>

using namespace std;

Player::Player() {
	hasOpponent = false;
	isWaiting = false;
}

Player& Player::GetOpponent() 
{
	return *opponent;
}

bool Player::IsWaiting()
{
	return isWaiting;
}

void Player::SetIsWaiting(bool value)
{
	isWaiting = value;
}

void Player::SetOpponent(Player *player) 
{
	opponent = player;	
	if (player == NULL)
		hasOpponent = false;
	else
		hasOpponent = true;
}

int Player::GetSocket()
{
	return socket;
}

void Player::SetSocket(int socket)
{
	this->socket = socket;
}

void Player::SetGame(Game *game)
{
	this->game = game;
}

bool Player::HasOpponent()
{
	return hasOpponent;
}

void Player::Act(char buffer[])
{
	string msg = buffer;


	if (msg.substr(0,4) == "MOVE")
	{
		int location = stoi(msg.substr(4,5));
		cout << "Move to: " << location << endl;
		if (game->IsValidMove(location))
		{
			game->MakeAMove(this, location);
			//send about valid move
			message = "MOVE" + to_string(location);
			if (game->HasWon())
			{
				message = "WIN";
				opponent->message = "LOSE";
			}
			else if (game->BoardIsFull())
			{
				message = "TIE";
				opponent->message = "TIE";
			}
		}
		else
		{
			message = "INVALID";
		}
	}
	else if (msg == "QUIT")
	{
		isWaiting = true;
	}
	else if (msg == "CONN")
	{
		message = "WAIT";
	}
	else
	{
		message = "INVALID";
	}
}