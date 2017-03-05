#include "stdafx.h"
#include "Player.h"

#include <iostream>
#include <string>

using namespace std;

Player::Player() {
	hasOpponent = false;
	isWaiting = false;
	isInvalid = false;
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
	
	char num = msg.at(0);
	if (num <= '9' && num >= '0')
//	if (msg.substr(0,4) == "MOVE")
	{
//		int location = stoi(msg.substr(4,5));
		int location = num - '0';

		if (game->IsValidMove(location))
		{
			game->MakeAMove(this, location);
			message = "MOVE" + to_string(location);
			opponent->message = "WAIT";
			if (game->HasWon())
			{
				message = "LOSE";
				opponent->message = "WIN";
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
	else if (msg.substr(0,4) == "EXIT")
	{
		opponent->SetIsWaiting(true);
		isWaiting = true;
	}
	else if (msg.substr(0, 4) == "QUIT")
	{
		message = "QUIT";
		opponent->SetIsWaiting(true);
		isWaiting = true;
		isInvalid = true;
	}
	else if (msg.substr(0,4) == "CONN")
	{
		message = "START";
		opponent->message = "WAIT";
	}
	else
	{
		isInvalid = true;
	}
}

void Player::Wait(char buffer[])
{
	string msg = buffer;

	if (msg.substr(0, 4) == "QUIT")
	{
		isWaiting = true;
		
	}
}