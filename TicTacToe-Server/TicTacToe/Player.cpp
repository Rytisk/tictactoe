#include "stdafx.h"
#include "Player.h"

Player::Player() {

}

Player& Player::GetOpponent() {
	return *opponent;
}

void Player::SetOpponent(Player *player) {
	opponent = player;
}