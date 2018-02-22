#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <ncurses.h> 

#include "priorityqueue.h"
#include "dijkstra.h"
#include "hero.h"
#include "monster.h"
#include "character.h"

#define dungeon_width 80
#define dungeon_height 21

typedef int bool;
#define true 1
#define false 0

typedef struct mapPiece {
  char symbol;
  int hardness;
} mapPiece_t;

typedef struct room {
  int xPos;
  int yPos;
  int xSize;
  int ySize;
} room_t;

typedef struct dungeon{
  int numRooms;
  room_t *roomArray;
  mapPiece_t dungeonArray[dungeon_height][dungeon_width];
  int dungeonWeight[dungeon_height][dungeon_width];
  int distanceTunnelMap[dungeon_height][dungeon_width];
  vertex_t vertexTunnelMap[dungeon_height][dungeon_width];
  vertex_t vertexMap[dungeon_height][dungeon_width];
  int distanceMap[dungeon_height][dungeon_width];
} dungeon_t;


void print_dungeon(dungeon_t *dungeon, characterQueue_t *characterQueue);
int weight_dungeon(dungeon_t *dungeon);
int full_distance_graph(dungeon_t *dungeon, character_t *hero);
int rooms_distance_graph(dungeon_t *dungeon, character_t *hero);
int print_distance_graph(dungeon_t *dungeon, character_t *hero);
void connect_rooms(dungeon_t *dungeon);
room_t generate_new_room();
bool place_rooms(dungeon_t *dungeon);
bool place_hardness(dungeon_t *dungeon);
dungeon_t build_dungeon();
int save_dungeon(dungeon_t dungeon);
dungeon_t load_dungeon();
int init_character_queue(int numMonsters, characterQueue_t *characterQueue, dungeon_t *dungeon);
int move_character(dungeon_t *dungeon, characterQueue_t *characterQueue, int turnNumber);
int check_win_condition(characterQueue_t *characterQueue);
int select_closest_distance(dungeon_t *dungeon, character_t *character, int canTunnel);
int main(int argc, char *argv[]);


#endif
