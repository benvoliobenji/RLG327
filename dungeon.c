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
#include "dungeon.h"

#define dungeon_width 80
#define dungeon_height 21

typedef int bool;
#define true 1
#define false 0
/*
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
  vertex_t *vertexTunnelMap[dungeon_height][dungeon_width];
  vertex_t *vertexMap[dungeon_height][dungeon_width];
  int distanceMap[dungeon_height][dungeon_width];
} dungeon_t;
*/

/*
typedef struct hero{
  char symbol;
  int xPos;
  int yPos;
  int speed = 10;
} hero_t;
*/


//This method prints the dungeon to the terminal
void print_dungeon(dungeon_t *dungeon, characterQueue_t *characterQueue)
{
  int i, j, k, characterSymbol;
  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  characterSymbol = 0;

	  for(k = 0; k < characterQueue->size; k++)
	    {
	      if(characterQueue->characterQueue[k].xPos == j && characterQueue->characterQueue[k].yPos == i)
		{
		  mvwprintw(stdscr, i + 1, j,"%c", characterQueue->characterQueue[k].symbol);
		  characterSymbol = 1;
		}
	    }

	  if(characterSymbol == 0)
	    {
	      mvwprintw(stdscr, i + 1, j,"%c", dungeon->dungeonArray[i][j].symbol);
	    }
	}

      //printf("\n");

    }

  //printf("\n");
}

/*A method to weight the dungeon*/
int weight_dungeon(dungeon_t *dungeon)
{
  int i, j;
  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j< dungeon_width; j++)
	{
	  //If the xPos and yPos are edges, the weight is infinity
	  if(dungeon->dungeonArray[i][j].hardness == 255)
	    {
	      dungeon->dungeonWeight[i][j] = 255;
	    }

	  //If the xPos and yPos are rooms or corridors, the weight is only 1
	  else if(dungeon->dungeonArray[i][j].hardness == 0)
	    {
	      dungeon ->dungeonWeight[i][j] = 1;
	    }

	  //Else, the weight is the hardness divided by 85 and adding 1
	  else
	    {
	      dungeon->dungeonWeight[i][j] = ((dungeon->dungeonArray[i][j].hardness) / 85) + 1;
	    }
	}
    }

  return 0;
}

/*The program to run dijkstra on monsters who can tunnel */
int full_distance_graph(dungeon_t *dungeon, character_t *hero)
{
  int i, j;
  int count = 0;
  vertex_t addVertex;
  vertex_t *vertexArray = (vertex_t*) calloc(dungeon_width * dungeon_height, sizeof(vertex_t));

  //printf("Adding vertex to vertexArray\n");


  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  //If it is a hero, change vertex accordingly
	  if(i == hero->yPos && j == hero->xPos)
	    {
	      addVertex.xPos = j;
	      addVertex.yPos = i;
	      addVertex.weight = 0;
	      addVertex.prev = NULL;
	      addVertex.distance = 0;
	      addVertex.visited = 0;
	      addVertex.priority = 0;

	    }

	  //Else the vertex is treated here
	  else
	    {
	      addVertex.xPos = j;
	      addVertex.yPos = i;
	      addVertex.weight = dungeon->dungeonWeight[i][j];
	      addVertex.prev = NULL;
	      addVertex.distance = 255;
	      addVertex.visited = 0;
	      addVertex.priority = 255;
	    }

	  vertexArray[count] = addVertex;

	  count++;

	}
    }

  //Do dijkstra
  dijkstra(vertexArray, count);

  //Update the distance map with the new distances
  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  dungeon->distanceTunnelMap[i][j] = vertexArray[i * dungeon_width + j].distance;
	  dungeon->vertexTunnelMap[i][j] = vertexArray[i * dungeon_width + j];
	}
    }

  free(vertexArray);

  return 0;
}

/*The program to run dijkstra for the monsters who cannot tunnel */
int rooms_distance_graph(dungeon_t *dungeon, character_t *hero)
{
  int i, j;
  int count = 1;
  vertex_t addVertex;
  vertex_t *vertexArray = (vertex_t*) calloc(count, sizeof(vertex_t));


  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  //If it is a hero, update the vertex accordingly
	  if(i == hero->yPos && j == hero->xPos)
	    {
	      addVertex.xPos = j;
	      addVertex.yPos = i;
	      addVertex.weight = dungeon->dungeonWeight[i][j];
	      addVertex.prev = NULL;
	      addVertex.distance = 0;
	      addVertex.visited = 0;
	      addVertex.priority = 0;

	    }

	  //If it is a room or corridor, keep the weight as normal
	  else if(dungeon->dungeonArray[i][j].hardness == 0)
	    {
	      addVertex.xPos = j;
	      addVertex.yPos = i;
	      addVertex.weight = dungeon->dungeonWeight[i][j];
	      addVertex.prev = NULL;
	      addVertex.distance = 255;
	      addVertex.visited = 0;
	      addVertex.priority = 255;
	    }

	  //Else, make it infinitely weighted
	  else
	    {
	      addVertex.xPos = j;
	      addVertex.yPos = i;
	      addVertex.weight = 255;
	      addVertex.prev = NULL;
	      addVertex.distance = 255;
	      addVertex.visited = 0;
	      addVertex.priority = 255;
	    }

	  vertexArray[count - 1] = addVertex;

	  count++;

	  vertexArray = realloc(vertexArray, count * sizeof(vertex_t));

	}
    }

  //Performing dijkstra's algorithm
  dijkstra(vertexArray, count);

  //Mapping the updated distances to the distance map
  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  dungeon->distanceMap[i][j] = vertexArray[i * dungeon_width + j].distance;
	  dungeon->vertexMap[i][j] = vertexArray[ i * dungeon_width + j];
	}
    }

  free(vertexArray);

  return 0;
}

/*This function takes the distance graphs and prints both of them to the screen */
int print_distance_graph(dungeon_t *dungeon, character_t *hero)
{
  int i, j;

  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{

	  if(dungeon->distanceTunnelMap[i][j] == 255)
	    {
	      printf("%c", dungeon->dungeonArray[i][j].symbol);
	    }

	  else if(i == hero->yPos && j == hero->xPos)
	    {
	      printf("%c", hero->symbol);
	    }

	  else
	    {
	      printf("%d", dungeon->distanceTunnelMap[i][j] % 10);
	    }
	}

      printf("\n");
    }

  printf("\n");

  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  if(dungeon->distanceMap[i][j] == 255)
	    {
	      printf("%c", dungeon->dungeonArray[i][j].symbol);
	    }

	  else if(i == hero->yPos && j == hero->xPos)
	    {
	      printf("%c", hero->symbol);
	    }

	  else
	    {
	      printf("%d", dungeon->distanceMap[i][j] % 10);
	    }
	}

      printf("\n");
    }

  printf("\n");
}

/*This function connects the rooms together. As of writing this, I chose the linear route
  for the sake of time, but may add dijkstra's algorithm later if I can */
void connect_rooms(dungeon_t *dungeon)
{
  room_t room, roomConnect;
  int y, x, roomNum, i, j;

  roomNum = 0;

  //As long as there are rooms to connect in the room array, the method will repeat
  while(roomNum < (dungeon->numRooms) - 1)
    {
      room = dungeon->roomArray[roomNum];
      roomConnect = dungeon->roomArray[roomNum + 1];

      x = room.xPos;
      y = roomConnect.yPos;

      //This room moves linearly down or up to the y position of the room to connect to
      if(room.yPos > y)
	{
	  for(i = room.yPos; i > y; i--)
	    {
	      if(dungeon->dungeonArray[i][x].hardness != 0)
		{
		  dungeon->dungeonArray[i][x].symbol = '#';
		  dungeon->dungeonArray[i][x].hardness = 0;
		}

	    }
	}
      else
	{
	  for(i = room.yPos; i < y; i++)
	    {
	      if(dungeon->dungeonArray[i][x].hardness != 0)
		{
		  dungeon->dungeonArray[i][x].symbol = '#';
		  dungeon->dungeonArray[i][x].hardness = 0;
		}
	    }

	}

      if(x > roomConnect.xPos)
	{
	  for(j = x; j > roomConnect.xPos; j--)
	    {
	      if(dungeon->dungeonArray[y][j].hardness != 0)
		{
		  dungeon->dungeonArray[y][j].symbol = '#';
		  dungeon->dungeonArray[y][j].hardness = 0;
		}
	    }
	}
      else
	{
	  for(j = x; j < roomConnect.xPos; j++)
	    {
	      if(dungeon->dungeonArray[y][j].hardness != 0)
		{
		  dungeon->dungeonArray[y][j].symbol = '#';
		  dungeon->dungeonArray[y][j].hardness = 0;
		}
	    }
	}

      roomNum++;
    }
}

//This method generates and returns a new room for the dungeon
room_t generate_new_room()
{

  int randNum;

  room_t newRoom;

  randNum = rand();
  //This will give me a random x-position between 1-79
  newRoom.xPos = (randNum % 79) + 1;

  randNum = rand();
  //This will give me a random y-position between 1-20
  newRoom.yPos = (randNum % 20) + 1;

  //This generates a room width below 12 units and above the minimum units
  randNum = (rand() % 10) + 3;
  newRoom.xSize = randNum;

  //This generates a room height below 6 units and above the minimum units
  randNum = (rand() % 5) + 2;
  newRoom.ySize = randNum;

  return newRoom;
}

bool place_rooms(dungeon_t *dungeon)
{
  int failedAttempts = 0;
  int  numRooms, i , j, k, areaOpen;
  bool firstTest, canBePlaced;
  room_t room1, room2, room3, room4, room5, newRoom, testRoom;

  //Purposefully designed the array to be much larger than possible to fill in this context
  room_t *roomArray = (room_t *) calloc(numRooms, sizeof(room_t));

  firstTest = false;
  areaOpen = 0;

  /*This first loop is designed to test the first 5 generated rooms to see if any overlap*/
  while(!firstTest)
    {
      room1 = generate_new_room();
      room2 = generate_new_room();
      room3 = generate_new_room();
      room4 = generate_new_room();
      room5 = generate_new_room();

      numRooms = 5;

      roomArray = realloc(roomArray, numRooms * sizeof(room_t));

      roomArray[0] = room1;
      roomArray[1] = room2;
      roomArray[2] = room3;
      roomArray[3] = room4;
      roomArray[4] = room5;

      canBePlaced = true;

      /*This set of for loops will go through each room in the room array and
	check to see if it overlaps with an outer wall or another room*/
      for(i = 0; i < numRooms; i++)
	{
	  testRoom = roomArray[i];

	  //This test is to make sure that all rooms are within the dungeon boundaries
	  if(testRoom.xPos + testRoom.xSize > 80 || testRoom.yPos + testRoom.ySize > 20)
	    {
	      free(roomArray);
	      return false;
	    }

	  for(j = testRoom.yPos; j < (testRoom.yPos + testRoom.ySize); j++)
	    {
	      for(k = testRoom.xPos; k < (testRoom.xPos + testRoom.xSize); k++)
		{
		  /*If it does find a conflict, it resets the dungeon, breaks the
		    loop, and tries again */
		  if(dungeon->dungeonArray[j][k].hardness == 0 || dungeon->dungeonArray[j][k].hardness == 255)
		    {
		      free(roomArray);
		      return false;
		    }

		  if(canBePlaced)
		    {
		      dungeon->dungeonArray[j][k].symbol = '.';
		      dungeon->dungeonArray[j][k].hardness = 0;
		      areaOpen++;
		    }
		}
	    }

	}
      firstTest = true;
    }


  canBePlaced = true;

  /*This loop will attempt to add a new room into the dungeon until it fails to
    place the room 2000 times or rooms cover 200 units  of the dungeon */
  while(failedAttempts < 2000 && areaOpen < 200)
    {

      newRoom = generate_new_room();
      canBePlaced = true;

      //This will test to see if a room can be placed inside the dungeon
      for(i = newRoom.yPos; i < newRoom.yPos + newRoom.ySize; i++)
	{
	  for(j = newRoom.xPos; j < newRoom.xPos + newRoom.xSize; j++)
	    {
	      if(dungeon->dungeonArray[i][j].hardness == 0 || dungeon->dungeonArray[i][j].hardness == 255)
		{
		  canBePlaced = false;
		}
	    }
	}

      //If the room can be placed without conflict, place the room
      if(canBePlaced)
	{
	  failedAttempts = 0;

	  for(i = newRoom.yPos; i < newRoom.yPos + newRoom.ySize; i++)
	    {
	      for(j = newRoom.xPos; j < newRoom.xPos + newRoom.xSize; j++)
		{
		  dungeon->dungeonArray[i][j].symbol = '.';
		  dungeon->dungeonArray[i][j].hardness = 0;
		  areaOpen++;
		}
	    }

	  roomArray = realloc(roomArray, (numRooms + 1) * sizeof(room_t));

	  //Add the room to the roomArray
	  roomArray[numRooms] = newRoom;
	  numRooms++;
	}

      else
       {
	failedAttempts++;
       }

    }

  dungeon->roomArray = realloc(dungeon->roomArray, numRooms * sizeof(room_t));

  for(i = 0; i < numRooms; i++)
    {
      dungeon->roomArray[i] = roomArray[i];
    }

  dungeon->numRooms = numRooms;

  free(roomArray);

  return true;
}

/*This method moves through the dungeon map, placing the border around the
  dungeon and then placing rock with random hardness within the map itself */
bool place_hardness(dungeon_t *dungeon)
{

  int i, j;
  int randHardness = 0;
        //This makes the left border of the dungeon an immutable wall
      for(i = 0; i < dungeon_height; i++)
	{
	  dungeon->dungeonArray[i][0].symbol = ' ';
	  dungeon->dungeonArray[i][0].hardness = 255;
	}

      //This makes the right border of the dungeon an immutable wall
      for(i = 0; i < dungeon_height; i++)
	{
	  dungeon->dungeonArray[i][dungeon_width - 1].symbol = ' ';
	  dungeon->dungeonArray[i][dungeon_width - 1].hardness = 255;
	}

      //This makes the top border of the dungeon an immutable wall
      for(j = 0; j < dungeon_width; j++)
	{
	  dungeon->dungeonArray[0][j].symbol = ' ';
	  dungeon->dungeonArray[0][j].hardness = 255;
	}

      //This make the bottom border of the dungeon an immutable wall
      for(j = 0; j < dungeon_width; j++)
	{
	  dungeon->dungeonArray[dungeon_height - 1][j].symbol = ' ';
	  dungeon->dungeonArray[dungeon_height - 1][j].hardness = 255;
	}

      /*This method moves through the rest of the array and assigns a random
	hardness to the rock inside the dungeon currently */
      for(i = 1; i < dungeon_height - 1; i++)
	{
	  for(j = 1; j < dungeon_width - 1; j++)
	    {
	      //This will give me a random number between 1-254
	      randHardness = (rand() % 254) + 1;

	      dungeon->dungeonArray[i][j].symbol = ' ';
	      dungeon->dungeonArray[i][j].hardness = randHardness;

	    }
	}

  return true;

}

/*This method is designed to create the raw dungeon, and then  build the dungeon */
dungeon_t build_dungeon()
{

  dungeon_t dungeon;
  mapPiece_t starter;
  int i, j;
  bool roomsPlaced = false;

  dungeon.roomArray = (room_t *) calloc(5, sizeof(room_t));

  starter.symbol = ' ';
  starter.hardness = 255;

  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  dungeon.dungeonArray[i][j] = starter;
	}
    }

  while(!roomsPlaced)
    {
      place_hardness(&dungeon);
      roomsPlaced = place_rooms(&dungeon);
    }

  weight_dungeon(&dungeon);
  connect_rooms(&dungeon);

  weight_dungeon(&dungeon);
  //print_dungeon(&dungeon);

  return dungeon;

}

/*Given a dungeon parameter, saves the dungeon to a binary file in a specific format.*/
int save_dungeon(dungeon_t dungeon)
{
  FILE *f;
  int version = 0;
  int size = (1700 + (dungeon.numRooms * 4));
  int i, j, length;
  char name[] = "RLG327-S2018";

  //This allows me to have 1 byte integers
  unsigned char xPos, yPos, xSize, ySize;

  //In order to have the integer hardness to be one byte, it must be an unsigned char
  //I also dynamically allocated the array, but that shouldn't be needed due to the nature of the dungeon size
  unsigned char*  mapHardness = (char*) malloc(dungeon_width * dungeon_height * sizeof(char));

  length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1;

  char *file = (char*) malloc(length * sizeof(char));

  strcpy(file, getenv("HOME"));
  strcat(file, "/.rlg327/dungeon");

  f = fopen(file, "wb");

  free(file);

  //Tests to see if the file was sucessfully created/opened
  if(f == NULL)
    {
      fprintf(stderr, "File could not be created or opened.\n");
      return -1;
    }

  //Translates the integer hardness in the dungeon to an unsigned char array in mapHardness
  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  mapHardness[i * dungeon_width + j] = (unsigned char) dungeon.dungeonArray[i][j].hardness;
	}
    }

  //Changes the version and size of the file to big endian
  version = htobe32(version);
  size = htobe32(size);

  //Writes the name, version, and size to the file
  fwrite(name, sizeof(char), 12, f);
  fwrite(&version, sizeof(int), 1, f);
  fwrite(&size, sizeof(int), 1, f);

  //Writes the hardness map to the file
  fwrite(mapHardness, sizeof(char), dungeon_width * dungeon_height, f);

  /*Steps through each room in the roomArray, changing each int value into unsigned char values and
    writes those values into the file */
  for(i = 0; i < dungeon.numRooms; i++)
    {
      yPos = (unsigned char) dungeon.roomArray[i].yPos;
      xPos = (unsigned char) dungeon.roomArray[i].xPos;
      ySize = (unsigned char) dungeon.roomArray[i].ySize;
      xSize = (unsigned char) dungeon.roomArray[i].xSize;

      fwrite(&yPos, sizeof(char), 1, f);
      fwrite(&xPos, sizeof(char), 1, f);
      fwrite(&ySize, sizeof(char), 1, f);
      fwrite(&xSize, sizeof(char), 1, f);

    }

  //Free's the allocated memory of mapHardness
  free(mapHardness);

  //Closes the file
  fclose(f);

  return 0;
}


/*This program loads the dungeon from the dungeon binary file and displays it on the screen*/
dungeon_t load_dungeon()
{
  dungeon_t dungeon;
  FILE* f;
  int version;
  int size;
  int length;
  int i, j, k, hardness;

  //Just like in save_dungeon, I used unsigned char in order to get integers into 1 byte
  unsigned char xPos, yPos, xSize, ySize;

  //Dynamically allocated, but relatively unnecessary
  char* name = (char*) calloc(12, sizeof(char));
  unsigned char* mapHardness = (char*)malloc(dungeon_height * dungeon_width * sizeof(char));

  length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1;

  char *file = (char*) malloc(length * sizeof(char));

  strcpy(file, getenv("HOME"));
  strcat(file, "/.rlg327/dungeon");

  f = fopen(file, "rb");

  free(file);

  //Testing to see if the file existed in the address defined above
  if(f == NULL)
    {
      fprintf(stderr, "File could not be open.\n");
      return dungeon;
    }

  fread(name, sizeof(char), 12, f);

  //Tests to see if the name of the file is correct
  if(strcmp(name, "RLG327-S2018"))
    {
      fprintf(stderr, "Incorrect file, cannot be read.\n");
      return dungeon;
    }

  //Reading in version and size
  fread(&version, sizeof(int), 1, f);
  fread(&size, sizeof(int), 1, f);

  //Converting version and size back from big endian
  version = be32toh(version);
  size = be32toh(size);

  /*Defining the number of rooms by subtracting the 12 char values of the name, the two 1 byte version
    and size, and the 80 * 21 1 byte hardness map and dividing by the number of values per room to get
    the number of rooms */
  dungeon.numRooms = (size - 1700) / 4;

  //Reading in the hardness map
  fread(mapHardness, sizeof(char), dungeon_width * dungeon_height, f);

  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  //Casting the hardness map unsigned char to an int
	  hardness = (int) mapHardness[i * dungeon_width + j];

	  //Assigning the hardness value into the hardness value of the dungeonArray
	  dungeon.dungeonArray[i][j].hardness = hardness;

	  dungeon.dungeonArray[i][j].symbol = ' ';
	}
    }

  //Dynamically allocating room for the roomArray based on the number of rooms and the size of room_t
  dungeon.roomArray = (room_t*)calloc(dungeon.numRooms, sizeof(room_t));

  //Changing each 1 byte unsigned char to integers and then assigning them to a specific room in roomArray
  for(i = 0; i < dungeon.numRooms; i++)
    {
      fread(&yPos, sizeof(char), 1, f);
      fread(&xPos, sizeof(char), 1, f);
      fread(&ySize, sizeof(char), 1, f);
      fread(&xSize, sizeof(char), 1, f);

      dungeon.roomArray[i].yPos = (int) yPos;
      dungeon.roomArray[i].xPos = (int) xPos;
      dungeon.roomArray[i].ySize = (int) ySize;
      dungeon.roomArray[i].xSize = (int) xSize;

      for(j = yPos; j < yPos + ySize; j++)
	{
	  for(k = xPos; k < xPos + xSize; k++)
	    {
	      dungeon.dungeonArray[j][k].hardness = 0;
	      dungeon.dungeonArray[j][k].symbol = '.';
	    }
	}
    }

  /* This then tests to see if any remaining cells in dungeonArray have a hardness of zero and don't have
     a symbol that indicates that they are a room. If those conditions are true, then the only thing that
     the cell can be is a corridor, and so the symbol is updated accordingly*/
  for(i = 0; i < dungeon_height; i++)
    {
      for(j = 0; j < dungeon_width; j++)
	{
	  if(dungeon.dungeonArray[i][j].hardness == 0 && dungeon.dungeonArray[i][j].symbol != '.')
	    {
	      dungeon.dungeonArray[i][j].symbol = '#';
	    }
	}
    }

  //Freeing name and mapHardnesss
  free(name);
  free(mapHardness);

  //Closing the file
  fclose(f);

  return dungeon;

}

//Initializes the character queue where the hero and monsters are
int init_character_queue(int numMonsters, characterQueue_t *characterQueue, dungeon_t *dungeon)
{
  character_t character;
  hero_t hero;
  monster_t monster;
  int xPos, yPos, i;

  //monster = malloc(sizeof(monster_t));
  //hero = malloc(sizeof(hero_t));

  printf("Allocating memory for CharacterQueue\n");
  //Allocating memory for the characterQueue
  characterQueue->characterQueue = (character_t *) calloc(numMonsters + 1, sizeof(character_t));
  characterQueue->size = 0;

  printf("Placing hero\n");

  character.hero = hero;
  character.speed = 10;
  //character.monster = NULL;

  xPos = (rand() % 79) + 1;
  yPos = (rand() % 20) + 1;

  while(dungeon->dungeonArray[yPos][xPos].hardness != 0 || dungeon->dungeonArray[yPos][xPos].symbol == '#')
    {
      xPos = (rand() % 79) + 1;
      yPos = (rand() % 20) + 1;
    }

  character.xPos = xPos;
  character.yPos = yPos;
  character.nextTurn = 0;
  character.dead = 0;

  character.symbol = '@';

  characterQueue->characterQueue[characterQueue->size] = character;

  characterQueue->size++;

  printf("Placing monsters\n");
  for(i = 0; i < numMonsters; i++)
    {
      printf("Determining monster traits\n");
      monster.traits = determine_traits();

      printf("Determined traits\n");
      //character.hero = NULL;
      character.monster = monster;
      character.dead = 0;

      printf("Monster symbol\n");
      switch(monster.traits){
      case 0:
	character.symbol = '0';
	break;

      case 1:
	character.symbol = '1';
	break;

      case 2:
	character.symbol = '2';
	break;

      case 3:
	character.symbol = '3';
	break;

      case 4:
	character.symbol = '4';
	break;

      case 5:
	character.symbol = '5';
	break;

      case 6:
	character.symbol = '6';
	break;

      case 7:
	character.symbol = '7';
	break;

      case 8:
	character.symbol = '8';
	break;

      case 9:
	character.symbol = '9';
	break;

      case 10:
	character.symbol = 'a';
	break;

      case 11:
	character.symbol = 'b';
	break;

      case 12:
	character.symbol = 'c';
	break;

      case 13:
	character.symbol = 'd';
	break;

      case 14:
	character.symbol = 'e';
	break;

      case 15:
	character.symbol = 'f';
	break;
      }

      printf("Monster speed\n");
      character.speed = (rand() % 16) + 5;

      xPos = (rand() % 79) + 1;
      yPos = (rand() % 20) + 1;

      printf("Determine xPos and yPos\n");
      while(dungeon->dungeonArray[yPos][xPos].hardness != 0 && dungeon->dungeonArray[yPos][xPos].symbol != '.')
	{
	  printf("Finding new xPos and yPos\n");
	  xPos = (rand() % 79) + 1;
	  yPos = (rand() % 20) + 1;

	  printf("xPos: %d yPos: %d\n", xPos, yPos);
	}

      character.xPos = xPos;
      character.yPos = yPos;
      character.nextTurn = 0;

      printf("Placing character in characterQueue\n");
      characterQueue->characterQueue[characterQueue->size] = character;
      characterQueue->size++;
    }

  return 0;

}

//Handles the movement of the hero and monsters
int move_character(dungeon_t *dungeon, characterQueue_t *characterQueue, int turnNumber)
{
  int i, found, randPos, xPos, yPos, j, validPosition, numRooms, updateX, updateY;
  int corridorCheck, seenInCorridor, windowListen, checkMenu;
  character_t character;

  i = 0;
  found = 0;
  checkMenu = 0;

  while(found == 0 && i < characterQueue->size)
    {
      character = characterQueue->characterQueue[i];
      //printf("Next Turn: %d\n", character.nextTurn);
      //printf("Dead: %d\n", character.dead);

      if(character.nextTurn <= turnNumber && character.dead == 0)
	{
	  //printf("Found\n");
	  found = 1;
	}

      else
	{
	  i++;
	}
    }

  if(found == 1)
    {
      if(i == 0)
	{
	  validPosition = 0;

	  while(validPosition == 0)
	    {
        c = getch(stdscr);

        checkMenu = 0;

        switch(c)
        {
          //If the user presses "y" or "7", move one up and one left
          case 121:
          case 55:
            xPos = character.xPos - 1;
            yPos = character.yPos - 1;
            break;

          //If the user presses "k" or "8", move one up
          case 107:
          case 56:
            xPos = character.xPos;
            yPos = character.yPos - 1;
            break;

          //If the user presses "u" or "9", move one up and one right
          case 117:
          case 57:
            xPos = character.xPos + 1;
            yPos = character.yPos - 1;
            break;

          //If the user presses "l" or "6", move one right
          case 108:
          case 54:
            xPos = character.xPos + 1;
            yPos = character.yPos;
            break;

          //If the user presses "n" or "3", move on down and one right
          case 110:
          case 51:
            xPos = character.xPos + 1;
            yPos = character.yPos + 1;
            break;

          //If the user presses "j" or "2", move one down
          case 106:
          case 50:
            xPos = character.xPos;
            yPos = character.yPos + 1;
            break;

          //If the user presses "b" or "1", move one down and one left
          case 98:
          case 49:
            xPos = character.xPos - 1;
            yPos = character.yPos + 1;
            break;

          //If the user presses "h" or "4", move one left
          case 104:
          case 52:
            xPos = character.xPos - 1;
            yPos = character.yPos;
            break;

          //If the user presses ">", attempt to go down stairs
          case 62:
            //Testing to see if hero is on downwards staircase
            if(dungeon->dungeonArray[character.yPos][character.xPos].symbol == '>')
            {
              //This will be the return signal to go downstairs
              return -2;
            }
            break;

          //If the user presses "<", attempt to go up stairs
          case 60:
            //Testing to see if hero is on upwards staircase
            if(dungeon->dungeonArray[character.yPos][character.xPos].symbol == '<')
            {
              //This will be the return signal to go upstairs
              return -3;
            }
            break;

          //If the user presses "5" or "Space", rest 1 turn
          case 53:
          case 32:
            xPos = character.xPos;
            yPos = character.yPos;
            break;

          //If the user presses "m", display monster list
          case 109:
            view_monster_list(character_t *characterQueue, dungeon_t* dungeon);
            checkMenu = 1;
            break;

          case 81:
            //This will be the return signal to quit
            return -1;
        }

	      if(xPos < 79 && yPos < 20 && xPos > 0 && yPos > 0 && checkMenu == 0)
		{
		  validPosition = 1;
		}
	    }

	  character.xPos = xPos;
	  character.yPos = yPos;

	  for(j = 1; j < characterQueue->size; j++)
	    {
	      //Defeating the monster
	      if(xPos == characterQueue->characterQueue[j].xPos && yPos == characterQueue->characterQueue[j].yPos)
		{
		  //printf("Dying to hero\n");
		  //characterQueue->characterQueue[j].monster = NULL;
		  characterQueue->characterQueue[j].symbol = NULL;
		  characterQueue->characterQueue[j].xPos = 256;
		  characterQueue->characterQueue[j].yPos = 256;
		  characterQueue->characterQueue[j].nextTurn = NULL;
		  characterQueue->characterQueue[j].speed = NULL;
		  characterQueue->characterQueue[j].dead = 1;
		}
	    }

	  //If moving into a wall
	  if(dungeon->dungeonArray[yPos][xPos].hardness != 0)
	    {
	      dungeon->dungeonArray[yPos][xPos].hardness = 0;
	      dungeon->dungeonArray[yPos][xPos].symbol = '#';
	    }

	  character.nextTurn = character.nextTurn + (1000 / character.speed);


	  characterQueue->characterQueue[0] = character;
	}

      else
	{
	  //printf("Moving Monster\n");
	  //printf("%d\n", character.monster.traits);
	  //printf("%c\n", character.symbol);
	  //printf("%d\n", (character.monster.traits >> 2) % 2);
	  if((int)character.monster.traits >> 3 == 1 && (rand() % 2) == 1)
	    {
	      //printf("Monster is erratic\n");
	      validPosition = 0;

	      while(validPosition == 0)
		{
		  randPos = rand() % 8;

		  switch(randPos)
		    {

		    case 0:
		      xPos = character.xPos;
		      yPos = character.yPos - 1;
		      break;

		    case 1:
		      xPos = character.xPos + 1;
		      yPos = character.yPos - 1;
		      break;

		    case 2:
		      xPos = character.xPos + 1;
		      yPos = character.yPos;

		    case 3:
		      xPos = character.xPos + 1;
		      yPos = character.yPos + 1;
		      break;

		    case 4:
		      xPos = character.xPos;
		      yPos = character.yPos + 1;
		      break;

		    case 5:
		      xPos = character.xPos - 1;
		      yPos = character.yPos + 1;
		      break;

		    case 6:
		      xPos = character.xPos - 1;
		      yPos = character.yPos;
		      break;

		    case 7:
		      xPos = character.xPos - 1;
		      yPos = character.yPos - 1;
		      break;
		    }

		  if(xPos < 79 && yPos < 20 && ((int)(character.monster.traits >> 1) % 2 == 1 || dungeon->dungeonArray[yPos][xPos].hardness == 0))
		    {
		      validPosition = 1;
		    }
		}

	      character.xPos = xPos;
	      character.yPos = yPos;
	    }

	  else if((int)(character.monster.traits >> 2) % 2 == 1)
	    {
	      //printf("Monster is telepathic\n");
	      if((int)(character.monster.traits >> 1) % 2 == 1)
		{
		  //printf("Monster can tunnel\n");

		  select_closest_distance(dungeon, &character, 1);
		}

	      else
		{
		  //printf("Monster can't tunnel\n");

		  select_closest_distance(dungeon, &character, 0);
		}
	    }

	  else if((character.monster.traits % 2 == 1))
	    {
	      //printf("Monster is intelligent\n");
	      for(numRooms = 0; numRooms < dungeon->numRooms; numRooms++)
		{
		  //Testing to see if the monster is in the same room as the hero
		  if((character.xPos >= dungeon->roomArray[numRooms].xPos && character.xPos <= (dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize) && character.yPos >= dungeon->roomArray[numRooms].yPos && character.yPos <= (dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize)) && (characterQueue->characterQueue[0].xPos >= dungeon->roomArray[numRooms].xPos && characterQueue->characterQueue[0].xPos <= (dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize) && characterQueue->characterQueue[0].yPos >= dungeon->roomArray[numRooms].yPos && characterQueue->characterQueue[0].yPos <= (dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize)))
		    {
		      //printf("Monster is in same room\n");
		      //Updating monster's memory
		      if((int)character.monster.traits >> 1 % 2 == 1)
			{
			  //printf("Monster can tunnel\n");
			  //printf("Updating monster memory\n");
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster.memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				}
			    }
			}

		      else
			{
			  //printf("Monster can't tunnel\n");
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster.memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
				}
			    }
			}
		    }
		}

	      //Check to see if the monster can see the hero in a vertical corridor
	      if(character.xPos == characterQueue->characterQueue[0].xPos)
		{
		  //printf("Monster is in vertical corridor\n");
		  seenInCorridor = 1;

		  if(character.yPos > characterQueue->characterQueue[0].yPos)
		    {
		      for(corridorCheck = characterQueue->characterQueue[0].yPos; corridorCheck <= character.yPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[corridorCheck][character.xPos].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}
		    }

		  else
		    {
		      for(corridorCheck = character.yPos; corridorCheck <= characterQueue->characterQueue[0].yPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[corridorCheck][character.xPos].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}
		    }

		  if(seenInCorridor == 1)
		    {
		      //printf("Monster can see hero\n");
		      //Updating monster's memory
		      if((int)character.monster.traits >> 1 % 2 == 1)
			{
			  //printf("Monster can tunnel\n");
			  //printf("Updating monster memory\n");
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster.memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				}
			    }
			}

		      else
			{
			  //printf("Monster can tunnel\n");
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster.memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
				}
			    }
			}
		    }
		}

	      //Check to see if the monster can see the hero in a horizontal corridor
	      else if(character.yPos == characterQueue->characterQueue[0].yPos)
		{
		  //printf("Monster can see a hero in a horizontal corridor\n");
		  seenInCorridor = 1;

		  if(character.xPos > characterQueue->characterQueue[0].xPos)
		    {
		      for(corridorCheck = characterQueue->characterQueue[0].xPos; corridorCheck <= character.xPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[character.yPos][corridorCheck].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}

		      if(seenInCorridor == 1)
			{
			  //Updating monster's memory
			  if((int)character.monster.traits >> 1 % 2 == 1)
			    {
			      //printf("Monster can tunnel\n");
			      for(updateY = 0; updateY < 21; updateY++)
				{
				  for(updateX = 0; updateX < 80; updateX++)
				    {
				      character.monster.memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				    }
				}
			    }

			  else
			    {
			      //printf("Monster can't tunnel\n");
			      for(updateY = 0; updateY < 21; updateY++)
				{
				  for(updateX = 0; updateX < 80; updateX++)
				    {
				      character.monster.memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
				    }
				}
			    }
			}
		    }

		  else
		    {
		      for(corridorCheck = character.xPos; corridorCheck <= characterQueue->characterQueue[0].xPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[character.yPos][corridorCheck].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}
		    }

		  if(seenInCorridor == 1)
		    {
		      //Updating monster's memory
		      if((int)character.monster.traits >> 1 % 2 == 1)
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster.memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				}
			    }
			}

		      else
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster.memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
				}
			    }
			}
		    }
		}

	      //If they don't see the hero, they stay dormant
	      if(character.monster.memory[character.yPos][character.xPos].prev == NULL)
		{
		  //printf("Dont see hero\n");
		  character.xPos = character.xPos;
		  character.yPos = character.yPos;
		}

	      //Otherwise they take the fastest route
	      else
		{
		  //printf("Taking fastest route\n");
		  character.xPos = character.monster.memory[character.yPos][character.xPos].prev->xPos;
		  character.yPos = character.monster.memory[character.yPos][character.xPos].prev->yPos;
		}
	    }

	    //Else if the monster is only unintelligent
	    else
	      {
		//printf("Monster is only unintelligent\n");
		for(numRooms = 0; numRooms < dungeon->numRooms; numRooms++)
		{
		  //Testing to see if the monster is in the same room as the hero
		  if((character.xPos >= dungeon->roomArray[numRooms].xPos && character.xPos <= dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize && character.yPos >= dungeon->roomArray[numRooms].yPos && character.yPos <= dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize) && (characterQueue->characterQueue[0].xPos >= dungeon->roomArray[numRooms].xPos && characterQueue->characterQueue[0].xPos <= dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize && characterQueue->characterQueue[0].yPos >= dungeon->roomArray[numRooms].yPos && characterQueue->characterQueue[0].yPos <= dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize))
		    {
		      //Move towards hero
		      if((int)character.monster.traits >> 1 % 2 == 1)
			{
			  if(character.xPos - characterQueue->characterQueue[0].xPos > 0)
			    {
			      character.xPos--;
			    }

			  if(characterQueue->characterQueue[0].xPos - character.xPos > 0)
			    {
			      character.xPos++;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos > 0)
			    {
			      character.yPos--;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos < 0)
			    {
			      character.yPos++;
			    }
			}

		      else
			{
			  if(character.xPos - characterQueue->characterQueue[0].xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos - 1].hardness == 0)
			    {
			      character.xPos--;
			    }

			  if(characterQueue->characterQueue[0].xPos - character.xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos + 1].hardness == 0)
			    {
			      character.xPos++;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos > 0 && dungeon->dungeonArray[character.yPos - 1][character.xPos].hardness == 0)
			    {
			      character.yPos--;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos < 0 && dungeon->dungeonArray[character.yPos + 1][character.xPos].hardness == 0)
			    {
			      character.yPos++;
			    }
			}
		    }
		}

	      //Check to see if the monster can see the hero in a vertical corridor
	      if(character.xPos == characterQueue->characterQueue[0].xPos)
		{
		  seenInCorridor = 1;

		  if(character.yPos > characterQueue->characterQueue[0].yPos)
		    {
		      for(corridorCheck = characterQueue->characterQueue[0].yPos; corridorCheck <= character.yPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[corridorCheck][character.xPos].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}
		    }

		  else
		    {
		      for(corridorCheck = character.yPos; corridorCheck <= characterQueue->characterQueue[0].yPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[corridorCheck][character.xPos].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}
		    }

		  if(seenInCorridor == 1)
		    {
		      //Moving monster
		      if((int)character.monster.traits >> 1 % 2 == 1)
			{
			  if(character.xPos - characterQueue->characterQueue[0].xPos > 0)
			    {
			      character.xPos--;
			    }

			  if(characterQueue->characterQueue[0].xPos - character.xPos > 0)
			    {
			      character.xPos++;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos > 0)
			    {
			      character.yPos--;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos < 0)
			    {
			      character.yPos++;
			    }
			}

		      else
			{
			  if(character.xPos - characterQueue->characterQueue[0].xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos - 1].hardness == 0)
			    {
			      character.xPos--;
			    }

			  if(characterQueue->characterQueue[0].xPos - character.xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos + 1].hardness == 0)
			    {
			      character.xPos++;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos > 0 && dungeon->dungeonArray[character.yPos - 1][character.xPos].hardness == 0)
			    {
			      character.yPos--;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos < 0 && dungeon->dungeonArray[character.yPos + 1][character.xPos].hardness == 0)
			    {
			      character.yPos++;
			    }
			}
		    }
		}

	      //Check to see if the monster can see the hero in a vertical corridor
	      else if(character.yPos == characterQueue->characterQueue[0].yPos)
		{
		  seenInCorridor = 1;

		  if(character.xPos > characterQueue->characterQueue[0].xPos)
		    {
		      for(corridorCheck = characterQueue->characterQueue[0].xPos; corridorCheck <= character.xPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[character.yPos][corridorCheck].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}

		      if(seenInCorridor == 1)
			{
			  //Moving monster
			  if((int)character.monster.traits >> 1 % 2 == 1)
			    {
			      if(character.xPos - characterQueue->characterQueue[0].xPos > 0)
				{
				  character.xPos--;
				}

			      if(characterQueue->characterQueue[0].xPos - character.xPos > 0)
				{
				  character.xPos++;
				}

			      if(character.yPos - characterQueue->characterQueue[0].yPos > 0)
				{
				  character.yPos--;
				}

			      if(character.yPos - characterQueue->characterQueue[0].yPos < 0)
				{
				  character.yPos++;
				}
			    }

			  else
			    {
			      if(character.xPos - characterQueue->characterQueue[0].xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos - 1].hardness == 0)
				{
				  character.xPos--;
				}

			      if(characterQueue->characterQueue[0].xPos - character.xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos + 1].hardness == 0)
				{
				  character.xPos++;
				}

			      if(character.yPos - characterQueue->characterQueue[0].yPos > 0 && dungeon->dungeonArray[character.yPos - 1][character.xPos].hardness == 0)
				{
				  character.yPos--;
				}

			      if(character.yPos - characterQueue->characterQueue[0].yPos < 0 && dungeon->dungeonArray[character.yPos + 1][character.xPos].hardness == 0)
				{
				  character.yPos++;
				}
			    }
			}
		    }

		  else
		    {
		      for(corridorCheck = character.xPos; corridorCheck <= characterQueue->characterQueue[0].xPos; corridorCheck++)
			{
			  if(dungeon->dungeonArray[character.yPos][corridorCheck].hardness != 0)
			    {
			      seenInCorridor = 0;
			    }
			}
		    }

		  if(seenInCorridor == 1)
		    {
		      //Moving monster
		      if((int)character.monster.traits >> 1 % 2 == 1)
			{
			  if(character.xPos - characterQueue->characterQueue[0].xPos > 0)
			    {
			      character.xPos--;
			    }

			  if(characterQueue->characterQueue[0].xPos - character.xPos > 0)
			    {
			      character.xPos++;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos > 0)
			    {
			      character.yPos--;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos < 0)
			    {
			      character.yPos++;
			    }
			}

		      else
			{
			  if(character.xPos - characterQueue->characterQueue[0].xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos - 1].hardness == 0)
			    {
			      character.xPos--;
			    }

			  if(characterQueue->characterQueue[0].xPos - character.xPos > 0 && dungeon->dungeonArray[character.yPos][character.xPos + 1].hardness == 0)
			    {
			      character.xPos++;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos > 0 && dungeon->dungeonArray[character.yPos - 1][character.xPos].hardness == 0)
			    {
			      character.yPos--;
			    }

			  if(character.yPos - characterQueue->characterQueue[0].yPos < 0 && dungeon->dungeonArray[character.yPos + 1][character.xPos].hardness == 0)
			    {
			      character.yPos++;
			    }
			}
		    }
		}

	      }

	    for(j = 0; j < characterQueue->size; j++)
	      {
		//printf("Testing to see if monster can defeat enemy\n");
		//printf("%d\n", j);
		//printf("%d\n", characterQueue->size);
		//Checking to see if the monster is moving into and defeating a hero
		if(character.xPos == characterQueue->characterQueue[j].xPos && character.yPos == characterQueue->characterQueue[j].yPos && character.symbol != characterQueue->characterQueue[j].symbol)
		  {
		    //printf("Dying to monsters\n");
		    //characterQueue->characterQueue[j].hero = NULL;
		    //characterQueue->characterQueue[j].monster = NULL;
		    characterQueue->characterQueue[j].symbol = NULL;
		    characterQueue->characterQueue[j].xPos = 256;
		    characterQueue->characterQueue[j].yPos = 256;
		    characterQueue->characterQueue[j].nextTurn = NULL;
		    characterQueue->characterQueue[j].speed = NULL;
		    characterQueue->characterQueue[j].dead = 1;
		  }
	      }

	    //If moving through a wall
	    if(dungeon->dungeonArray[character.yPos][character.xPos].hardness != 0)
	      {
		dungeon->dungeonArray[character.yPos][character.xPos].hardness = 0;
		dungeon->dungeonArray[character.yPos][character.xPos].symbol = '#';
	      }

	    //printf("Updating nextTurn\n");
	    character.nextTurn = character.nextTurn + (1000 / character.speed);

	    //printf("Putting character back into characterQueue\n");
	    characterQueue->characterQueue[i] = character;
	}
    }
  return turnNumber + 1;
}

//Checks to see if the player has won or lost the game
int check_win_condition(characterQueue_t *characterQueue)
{
  int i;

  if(characterQueue->characterQueue[0].dead == 1)
    {
      return -1;
    }

  else
    {
      for(i = 1; i < characterQueue->size; i++)
	{
	  if(characterQueue->characterQueue[i].dead != 1)
	    {
	      return 0;
	    }
	}

      return 1;
    }
}

//Given a character and the dungeon, selects the y and x coordinates with the shortest distance to the hero
int select_closest_distance(dungeon_t *dungeon, character_t *character, int canTunnel)
{
  int minDistance;

  if(canTunnel == 1)
    {
      minDistance = dungeon->distanceTunnelMap[character->yPos][character->xPos];

      if(dungeon->distanceTunnelMap[character->yPos -1][character->xPos] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos - 1][character->xPos];

	  character->yPos = character->yPos - 1;
	  character->xPos = character->xPos;
	}

      if(dungeon->distanceTunnelMap[character->yPos -1][character->xPos + 1] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos - 1][character->xPos + 1];

	  character->yPos = character->yPos - 1;
	  character->xPos = character->xPos + 1;
	}

      if(dungeon->distanceTunnelMap[character->yPos][character->xPos + 1] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos][character->xPos + 1];

	  character->yPos = character->yPos;
	  character->xPos = character->xPos + 1;
	}

      if(dungeon->distanceTunnelMap[character->yPos + 1][character->xPos + 1] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos + 1][character->xPos + 1];

	  character->yPos = character->yPos + 1;
	  character->xPos = character->xPos + 1;
	}

      if(dungeon->distanceTunnelMap[character->yPos + 1][character->xPos] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos + 1][character->xPos];

	  character->yPos = character->yPos + 1;
	  character->xPos = character->xPos;
	}

      if(dungeon->distanceTunnelMap[character->yPos + 1][character->xPos - 1] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos + 1][character->xPos - 1];

	  character->yPos = character->yPos + 1;
	  character->xPos = character->xPos - 1;
	}

      if(dungeon->distanceTunnelMap[character->yPos][character->xPos - 1] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos][character->xPos - 1];

	  character->yPos = character->yPos;
	  character->xPos = character->xPos - 1;
	}

      if(dungeon->distanceTunnelMap[character->yPos -1][character->xPos - 1] < minDistance)
	{
	  minDistance = dungeon->distanceTunnelMap[character->yPos - 1][character->xPos - 1];

	  character->yPos = character->yPos - 1;
	  character->xPos = character->xPos - 1;
	}
    }

  else
    {
      minDistance = dungeon->distanceMap[character->yPos][character->xPos];

      if(dungeon->distanceMap[character->yPos -1][character->xPos] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos - 1][character->xPos];

	  character->yPos = character->yPos - 1;
	  character->xPos = character->xPos;
	}

      if(dungeon->distanceMap[character->yPos -1][character->xPos + 1] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos - 1][character->xPos + 1];

	  character->yPos = character->yPos - 1;
	  character->xPos = character->xPos + 1;
	}

      if(dungeon->distanceMap[character->yPos][character->xPos + 1] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos][character->xPos + 1];

	  character->yPos = character->yPos;
	  character->xPos = character->xPos + 1;
	}

      if(dungeon->distanceMap[character->yPos + 1][character->xPos + 1] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos + 1][character->xPos + 1];

	  character->yPos = character->yPos + 1;
	  character->xPos = character->xPos + 1;
	}

      if(dungeon->distanceMap[character->yPos + 1][character->xPos] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos + 1][character->xPos];

	  character->yPos = character->yPos + 1;
	  character->xPos = character->xPos;
	}

      if(dungeon->distanceMap[character->yPos + 1][character->xPos - 1] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos + 1][character->xPos - 1];

	  character->yPos = character->yPos + 1;
	  character->xPos = character->xPos - 1;
	}

      if(dungeon->distanceMap[character->yPos][character->xPos - 1] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos][character->xPos - 1];

	  character->yPos = character->yPos;
	  character->xPos = character->xPos - 1;
	}

      if(dungeon->distanceMap[character->yPos -1][character->xPos - 1] < minDistance)
	{
	  minDistance = dungeon->distanceMap[character->yPos - 1][character->xPos - 1];

	  character->yPos = character->yPos - 1;
	  character->xPos = character->xPos - 1;
	}
    }

  return 0;
}

int new_character_queue(int numMonsters, characterQueue_t *characterQueue, dungeon_t *dungeon)
{
  monster_t monster;
  characterQueue->characterQueue[0].nextTurn = 0;
  characterQueue->size = 1;

  //printf("Placing monsters\n");
  for(i = 0; i < numMonsters; i++)
    {
      //printf("Determining monster traits\n");
      monster.traits = determine_traits();

      //printf("Determined traits\n");
      //character.hero = NULL;
      character.monster = monster;
      character.dead = 0;

      //printf("Monster symbol\n");
      switch(monster.traits){
      case 0:
	character.symbol = '0';
	break;

      case 1:
	character.symbol = '1';
	break;

      case 2:
	character.symbol = '2';
	break;

      case 3:
	character.symbol = '3';
	break;

      case 4:
	character.symbol = '4';
	break;

      case 5:
	character.symbol = '5';
	break;

      case 6:
	character.symbol = '6';
	break;

      case 7:
	character.symbol = '7';
	break;

      case 8:
	character.symbol = '8';
	break;

      case 9:
	character.symbol = '9';
	break;

      case 10:
	character.symbol = 'a';
	break;

      case 11:
	character.symbol = 'b';
	break;

      case 12:
	character.symbol = 'c';
	break;

      case 13:
	character.symbol = 'd';
	break;

      case 14:
	character.symbol = 'e';
	break;

      case 15:
	character.symbol = 'f';
	break;
      }

      //printf("Monster speed\n");
      character.speed = (rand() % 16) + 5;

      xPos = (rand() % 79) + 1;
      yPos = (rand() % 20) + 1;

      //printf("Determine xPos and yPos\n");
      while(dungeon->dungeonArray[yPos][xPos].hardness != 0 && dungeon->dungeonArray[yPos][xPos].symbol != '.')
	{
	  //printf("Finding new xPos and yPos\n");
	  xPos = (rand() % 79) + 1;
	  yPos = (rand() % 20) + 1;

	  //printf("xPos: %d yPos: %d\n", xPos, yPos);
	}

      character.xPos = xPos;
      character.yPos = yPos;
      character.nextTurn = 0;

      //printf("Placing character in characterQueue\n");
      characterQueue->characterQueue[characterQueue->size] = character;
      characterQueue->size++;
    }

  return 0;
}

int view_monster_list(characterQueue_t *characterQueue, dungeon_t *dungeon)
{
  int ch, i, xRelative, yRelative;
  character_t monster;

  //Create a new window with height of 10, width of 40, and at row 5 col 20
  WINDOW *monsterWindow = newwin(10, 40, 5, 20);

  for(i = 1; i < characterQueue->size; i++)
  {
    //Finding the x and y position of the monster relative to the hero
    xRelative = characterQueue->characterQueue[0].xPos - characterQueue->characterQueue[i].xPos;
    yRelative = characterQueue->characterQueue[0].yPos - characterQueue->characterQueue[i].yPos;

    //If the monster is south and east of the hero
    if(xRelative < 0 && yRelative < 0)
    {
      xRelative = characterQueue->characterQueue[i].xPos - characterQueue->characterQueue[0].xPos;
      yRelative = characterQueue->characterQueue[i].yPos - characterQueue->characterQueue[0].yPos;

      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units East and %d units South", characterQueue->characterQueue[i].symbol, xRelative, yRelative);
    }

    //If the monster is south and west of the hero
    else if(xRelative > 0 && yRelative < 0)
    {
      yRelative = characterQueue->characterQueue[i].yPos - characterQueue->characterQueue[0].yPos;
      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units West and %d units South", characterQueue->characterQueue[i].symbol, xRelative, yRelative);
    }

    //If the monster is east of the hero
    else if(xRelative < 0 && yRelative == 0)
    {
      xRelative = characterQueue->characterQueue[i].xPos - characterQueue->characterQueue[0].xPos;

      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units East", characterQueue->characterQueue[i].symbol, xRelative);
    }

    //If the monster is west of the hero
    else if(xRelative > 0 && yRelative == 0)
    {
      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units West", characterQueue->characterQueue[i].symbol, xRelative);
    }

    //If the monster is north and east of the hero
    else if(xRelative < 0 && yRelative > 0)
    {
      xRelative = characterQueue->characterQueue[i].xPos - characterQueue->characterQueue[0].xPos;

      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units East and %d units North", characterQueue->characterQueue[i].symbol, xRelative, yRelative);
    }

    //If the monster is north and west of the hero
    else if(xRelative > 0 && yRelative > 0)
    {
      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units West and %d units North", characterQueue->characterQueue[i].symbol, xRelative, yRelative);
    }

    //If the monster is south of the hero
    else if(xRelative == 0 && yRelative < 0)
    {
      yRelative = characterQueue->characterQueue[i].yPos - characterQueue->characterQueue[0].yPos;

      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units South", characterQueue->characterQueue[i].symbol, yRelative);
    }

    //If the monster is north of the hero
    else if(xRelative == 0 && yRelative > 0)
    {
      mvwprintw(monsterWindow, i - 1, 0, "A %c is %d units North", characterQueue->characterQueue[i].symbol, yRelative);
    }
  }

  //Refresh window
  wrefresh(monsterWindow);

  while(1)
  {
    ch = getch(monsterWindow);

    switch (ch) {
      //If the user wants to scroll up
      case 259:
        wscrl(monsterWindow, 1);
        break;

      case 258:
        wscrl(monsterWindow, -1);
        break;

      case 27:
        delwin(monsterWindow);
        return 0;
    }

    wrefresh(monsterWindow);
  }
}

int place_staircase(dungeon_t *dungeon)
{
  int xPos, yPos;

  xPos = rand() % 80;
  yPos = rand() % 21;

  while(dungeon->dungeonArray[yPos][xPos].hardness != 0)
  {
    xPos = rand() % 80;
    yPos = rand() % 21;
  }

  dungeon->dungeonArray[yPos][xPos].symbol = '>';

  xPos = rand() % 80;
  yPos = rand() % 21;

  while(dungeon->dungeonArray[yPos][xPos].hardness != 0)
  {
    xPos = rand() % 80;
    yPos = rand() % 21;
  }

  dungeon->dungeonArray[yPos][xPos].symbol = '<';

  return 0;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  dungeon_t dungeon;
  characterQueue_t characterQueue;
  int gameOver, turnNum, numMon, i;

  gameOver = 0;
  turnNum = 0;
  numMon = 10;

  //Initializes the window
  initscr(void);
  raw(void);
  noecho(void);
  curs_set(0);
  keypad(stdscr, TRUE);

  //If there are no save or load arguments, just work normally
  if(argc != 2 && argc != 3 && argc != 4 && argc != 5)
    {
      dungeon = build_dungeon();
    }

  //Else, test if the correct arguments are provided
  else if(argc == 2)
    {
        if(argv[1][0] == '-'&& argv[1][1] == '-')
	  {
	    if(argv[1][2] == 's')
	      {
		dungeon = build_dungeon();
		save_dungeon(dungeon);
	      }

	    else if(argv[1][2] == 'l')
	      {
		dungeon = load_dungeon();
	      }

	    else
	      {
		fprintf(stderr, "Wrong parameter.\n");
		return -1;
	      }
	  }

	else
	  {
	    fprintf(stderr, "Incorrect argument.\n");
	    return -1;
	  }
    }

  else if(argc == 3)
    {
      if(argv[1][0] == '-' && argv[1][1] == '-' && argv[2][0] == '-' && argv[2][1] == '-')
	{
	  if(argv[1][2] == 's')
	    {
	      if(argv[2][2] == 'l')
		{
		  dungeon = load_dungeon();
		  save_dungeon(dungeon);
		}

	      else
		{
		  fprintf(stderr, "Incorrect argument.\n");
		  return -1;
		}
	    }

	  else if(argv[1][2] == 'l')
	    {
	      if(argv[2][2] == 's')
		{
		  dungeon = load_dungeon();
		  save_dungeon(dungeon);
		}

	      else
		{
		  fprintf(stderr, "Incorrect argument.\n");
		  return -1;
		}
	    }

	  else
	    {
	      fprintf(stderr, "Incorrect argument.\n");
	      return -1;
	    }
	}

      else if(argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] == 'n')
	{
	  numMon = atoi(argv[2]);
	}

      else
	{
	  fprintf(stderr, "Incorrect argument.\n");
	  return -1;
	}
    }

  else if(argc == 4)
    {
      if(argv[1][0] == '-' && argv[1][1] == '-' && argv[2][0] == '-' && argv[2][1] == '-')
	{
	  if(argv[1][2] == 'l')
	    {
	      if(argv[2][2] == 'n')
		{
		  dungeon = load_dungeon();
		  numMon = atoi(argv[3]);
		}
	      else
		{
		  fprintf(stderr, "Incorrect argument.\n");
		  return -1;
		}
	    }

	  else if(argv[1][2] == 's')
	    {
	      if(argv[2][2] == 'n')
		{
		  dungeon = build_dungeon();
		  save_dungeon(dungeon);
		  numMon = atoi(argv[3]);
		}

	      else
		{
		  fprintf(stderr, "Incorrect argument.\n");
		  return -1;
		}
	    }

	  else
	    {
	      fprintf(stderr, "Incorrect argument.\n");
	      return -1;
	    }
	}

	else if(argv[1][0] == '-' && argv[1][1] == '-' && argv[3][0] == '-' && argv[3][1] == '-' && argv[1][2] == 'n')
	  {
	    numMon = atoi(argv[2]);

	    if(argv[3][2] == 'l')
	      {
		dungeon = load_dungeon();
	      }

	    else if(argv[3][2] == 's')
	      {
		dungeon = build_dungeon();
		save_dungeon(dungeon);
	      }

	    else
	      {
		fprintf(stderr, "Incorrect argument.\n");
		return -1;
	      }
	  }

	else
	  {
	    fprintf(stderr, "Incorrect argument.\n");
	    return -1;
	  }
    }
  //If there are all three arguments, it tests to see if they are formatted correctly, then runs both save, load, and placing monsters
  else
    {
      if(argv[1][0] == '-'&& argv[1][1] == '-' && argv[2][0] == '-' && argv[2][1] == '-' && argv[3][0] == '-' && argv[3][1] == '-')
	  {
	    numMon = atoi(argv[4]);
	    dungeon = load_dungeon();
	    save_dungeon(dungeon);
	  }

      else if(argv[1][0] == '-' && argv[1][1] == '-' && argv[2][0] == '-' && argv[2][1] == '-' && argv[4][0] == '-' && argv[4][1] == '-')
	{
	  numMon = atoi(argv[3]);
	  dungeon = load_dungeon();
	  save_dungeon(dungeon);
	}

      else if(argv[1][0] == '-' && argv[1][1] == '-' && argv[3][0] == '-' && argv[3][1] == '-' && argv[4][0] == '-' && argv[4][1] == '-')
	{
	  numMon = atoi(argv[2]);
	  dungeon = load_dungeon();
	  save_dungeon(dungeon);
	}

      else
	{
	  fprintf(stderr, "Wrong parameter.\n");
	  return -1;
	}
    }

  weight_dungeon(&dungeon);

  init_character_queue(numMon, &characterQueue, &dungeon);

  printf("Doing full distance graph\n");

  full_distance_graph(&dungeon, &characterQueue.characterQueue[0]);

  printf("Doing rooms distance graph\n");
  rooms_distance_graph(&dungeon, &characterQueue.characterQueue[0]);

  while(gameOver == 0)
    {

      print_dungeon(&dungeon, &characterQueue);
      turnNum = move_character(&dungeon, &characterQueue, turnNum);

      if(turnNum == -1)
      {
        printw("Sorry that you had to leave, didn't know our hero was a quitter\n");
        return 0;
      }
      if(turnNum == -2 || turnNum == -3)
      {
        turnNum = 0;
        dungeon = build_dungeon();
        new_character_queue(numMon, &characterQueue, &dungeon);
      }
      weight_dungeon(&dungeon);
      full_distance_graph(&dungeon, &characterQueue.characterQueue[0]);
      rooms_distance_graph(&dungeon, &characterQueue.characterQueue[0]);
      gameOver = check_win_condition(&characterQueue);
      refresh();
    }

  if(gameOver == -1)
    {
      printf("You Lost\n");
    }

  else
    {
      printf("You Won!\n");
    };


  free(dungeon.roomArray);
  free(characterQueue.characterQueue);

  return 0;
}
