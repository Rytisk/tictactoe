#include "stdafx.h"
#include "Player.h"

#include <iostream>

using namespace std;

Player::Player() {

}

Player& Player::GetOpponent() 
{
	return *opponent;
}

void Player::SetOpponent(Player *player) 
{
	opponent = player;
}

int Player::GetSocket()
{
	return socket;
}

void Player::SetSocket(int sock)
{
	socket = sock;
}

void Player::Act(char message[])
{
	cout << "Player got: " << message << endl;
}