#include <ncurses.h>
#include "dave.h"
#include "io.h"
#include "pc.h"

dave::dave() {
  //Changed this from a dice roll to an int to better balance rewards
  //Max explosion damage is a collosal 1250 damage
  dice *explosion = new dice((uint32_t) 50, (uint32_t) 60, (uint32_t) 20);
  int reward;
  int explosive = explosion->roll();
  
  if (explosive < 250) {
    reward = rand() % 100 + 50;
  }
  else if (explosive < 500) {
    reward = rand() % 300 + 100;
  }
  else if (explosive < 750) {
    reward = rand() % 1000 + 250;
  }
  else {
    reward = rand() % 10000 + 500;
  }
  
  this->explosives = explosive;
  this->gold = reward;
  this->following = false;
  this->symbol = 'D';
  this->speed = PC_SPEED;
  this->alive = 1;
  this->color.push_back(COLOR_BLUE);
  this->hp = 50;
  this->damage = new dice((uint32_t) 0, (uint32_t) 2, (uint32_t) 10);
  this->name = "Dave";
}

void dave::explode(dungeon *d)
{
  int damage = this->explosives;

  io_queue_message("The explosion does a whopping %d damage!", damage);

  int explosive_X_min = this->position[dim_x] - 2 >= 0 ? this->position[dim_x] - 2 : 0;
  int explosive_X_max = this->position[dim_x] + 2 < DUNGEON_X ? this->position[dim_x] + 2 : DUNGEON_X - 1;
  int explosive_Y_min = this->position[dim_y] - 2 >= 0 ? this->position[dim_y] - 2 : 0;
  int explosive_Y_max = this->position[dim_y] + 2 < DUNGEON_Y ? this->position[dim_y] + 2 : DUNGEON_Y - 1;

  for (int i = explosive_Y_min; i <= explosive_Y_max; i++) {
    for (int j = explosive_X_min; j <= explosive_X_max; j++) {
      if(d->character_map[i][j]) {
	if (damage >= (int)d->character_map[i][j]->hp) {
	  d->character_map[i][j]->hp = 0;
	  d->character_map[i][j]->alive = 0;
	  if (d->character_map[i][j]->symbol == 'D' && (this->position[dim_y] != i && this->position[dim_x] != j)) {
	    dave *exploded_dave = (dave *)d->character_map[i][j];
	    exploded_dave->explode(d);
	  }
	  else if(d->PC->position[dim_y] == i && d->PC->position[dim_x] == j) {
	    d->num_monsters--;
	  }
	
	  d->character_map[i][j] = NULL;
	}
	else {
	  d->character_map[i][j]->hp -= damage;
	}
      }
    }
  }

  d->character_map[this->position[dim_y]][this->position[dim_x]] = NULL;
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

int dave::show_reward() {
  return this->gold;
}

int dave::show_explosives() {
  return this->explosives;
}

void gen_dave(dungeon *d) {
  int xPos;
  int yPos;
  dave *new_dave;
	
  for(int i = 0; i < 10; i++) {
    
    do {
      xPos = rand() % DUNGEON_X;
      yPos = rand() % DUNGEON_Y;
    } while(d->character_map[yPos][xPos] || d->hardness[yPos][xPos] != 0);

    new_dave = new dave();
	
    new_dave->position[dim_x] = xPos;
    new_dave->position[dim_y] = yPos;
    
    d->character_map[yPos][xPos] = new_dave;
    
  }
  
}
