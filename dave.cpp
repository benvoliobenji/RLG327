#include "dave.h"

dave::dave() {
	dice *explosion = new dice((uint_t) 50, (uint_t) 30, (uint_t) 20);
	int reward;
	
	if (explosion->roll() < 100) {
		reward = rand() % 100;
	}
	else if (explosion->roll() < 250) {
		reward = rand() % 1000;
	}
	else {
		reward = rand() % 10000;
	}

	this->explosives = explosion;
	this->gold = reward;
}

int dave::explode()
{
	return this->explosives->roll();
}

int dave::give_reward()
{
	return this->gold;
}
