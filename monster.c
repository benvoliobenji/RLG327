#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "priorityqueue.h"
#include "monster.h"

//srand(time(NULL));

/*
typedef struct monster{
  unsigned char traits;
  vertex_t *memory[21][80];
} monster_t;
*/

int determine_traits()
{
  int intelligence, telepathy, tunnelingAbility, erraticBehavior;

  intelligence = rand() % 2;
  telepathy = rand() % 2;
  tunnelingAbility = rand() % 2;
  erraticBehavior = rand() % 2;

  telepathy <<= 1;
  tunnelingAbility <<= 2;
  erraticBehavior <<= 3;

  return intelligence + telepathy + tunnelingAbility + erraticBehavior;
}
