#ifndef MONSTER_H
#define MONSTER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "priorityqueue.h"

typedef struct monster{
  int traits;
  vertex_t memory[21][80];
} monster_t;

int determine_traits();

#endif
