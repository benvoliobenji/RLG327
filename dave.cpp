#include "dave.h"

dave::dave() {
	dice *explosion = new dice((uint32_t) 50, (uint32_t) 30, (uint32_t) 20);
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
	this->following = false;
	this->symbol = 'D';
	this->speed = PC_SPEED;
	this->alive = 1;
	this->color.push_back(COLOR_BLUE);
	this->hp = 200;
	this->damage = new dice((uint32_t) 0, (uint32_t) 2, (uint32_t) 10);
	this->name = "Dave";
}

void dave::explode(dungeon *d)
{
	int damage = this->explosives->roll();

	int explosive_X_min = this->position[dim_x] - 2 >= 0 ? this->position[dim_x] - 2 : 0;
	int explosive_X_max = this->position[dim_x] + 2 < DUNGEON_X ? this->position[dim_x] + 2 : DUNGEON_X - 1;
	int explosive_Y_min = this->position[dim_y] - 2 >= 0 ? this->position[dim_y] - 2 : 0;
	int explosive_Y_max = this->position[dim_y] + 2 < DUNGEON_Y ? this->position[dim_y] + 2 : DUNGEON_Y - 1;

	for (int i = explosive_Y_min; i <= explosive_Y_max; i++) {
		for (int j = explosive_X_min; j <= explosive_X_max; j++) {
			if (damage >= d->character_map[i][j]->hp) {
				d->character_map[i][j]->hp = 0;
				d->character_map[i][j]->alive = 0;
				if (d->character_map[i][j]->name == "Dave") {
					dave *exploded_dave = (dave *)d->character_map[i][j];
					exploded_dave->explode(d);
				}

				d->character_map[i][j] = NULL;
			}
			else {
				d->character_map[i][j]->hp -= damage;
			}
		}
	}
}

int dave::give_reward()
{
	this->following = false;
	return this->gold;
}

void dave::follow() {
	this->following = true;
}

bool dave::is_following()
{
	return this->following;
}

void gen_dave(dungeon *d) {
	int xPos;
	int yPos;
	dave *new_dave;

	for(int i = 0; i < 10; i++) {
		bool placed = false;

		while(!placed) {
			xPos = rand() % DUNGEON_X;
			yPos = rand() % DUNGEON_Y;

			if(!d->character_map[xPos][yPos]) {
				new_dave = new dave();

				new_dave->position[dim_x] = xPos;
				new_dave->position[dim_y] = yPos;

				pair_t next_pos;
				next_pos[dim_x] = xPos;
				next_pos[dim_y] = yPos;
				new_dave->set_next_pos(next_pos);

				d->character_map[xPos][yPos] = new_dave;

				placed = true;
			}

		}
	}

}
