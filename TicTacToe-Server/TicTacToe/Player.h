#pragma once
class Player {
private:
	Player * opponent;
public:
	Player& GetOpponent();
	void SetOpponent(Player *player);
	Player();
};