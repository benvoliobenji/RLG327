#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdio.h>
#include <stdlib.h>

#include "hero.h"
#include "monster.h"


typedef struct character{
  char symbol;
  int xPos;
  int yPos;
  int speed;
  hero_t hero;
  monster_t monster;
  int nextTurn;
  int dead;
}character_t;

typedef struct characterQueue{
  int size;
  character_t *characterQueue;
}characterQueue_t;

/*
int init_character_queue(int numMonsters, characterQueue_t *characterQueue, dungeon_t *dungeon);


int move_character(dungeon_t *dungeon, characterQueue_t *characterQueue, int turnNumber);

int check_win_condition(characterQueue_t *characterQueue);
*/
#endif
