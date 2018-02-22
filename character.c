#include <stdio.h>
#include <stdlib.h>

#include "hero.h"
#include "monster.h"
#include "dungeon.h"
#include "character.h"

/*
typedef struct character{
  char symbol;
  int xPos;
  int yPos;
  int speed;
  hero_t *hero;
  monster_t *monster;
  int nextTurn;
}character_t;


typedef struct characterQueue{
  int size;
  character_t *characterQueue;
} characterQueue_t;


int init_character_queue(int numMonsters, characterQueue_t *characterQueue, dungeon_t *dungeon)
{
  character_t character;
  hero_t *hero;
  monster_t *monster;
  int xPos, yPos, i, j;

  //Allocating memory for the characterQueue
  characterQueue->characterQueue = (character_t *) calloc(numMonsters + 1, sizeof(character_t));
  characterQueue->size = 0;

  character.hero = hero;
  character.speed = 10;
  character.monster = NULL;

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

  character.symbol = '@';

  characterQueue->characterQueue[characterQueue->size] = character;

  characterQueue->size++;

  for(i = 0; i < numMonsters; i++)
    {
      determine_traits(monster);

      character.hero = NULL;
      character.monster = monster;
      
      character.symbol = monster->traits;

      character.speed = (rand() % 16) + 5;

      xPos = (rand() % 79) + 1;
      yPos = (rand() % 20) + 1;

      while(dungeon->dungeonArray[yPos][xPos].hardness != 0 || dungeon->dungeonArray[yPos][xPos].symbol != '.')
	{
	  xPos = (rand() % 79) + 1;
	  yPos = (rand() % 20) + 1;
	}

      character.xPos = xPos;
      character.yPos = yPos;
      character.nextTurn = 0;

      characterQueue->characterQueue[characterQueue->size] = character;
      characterQueue->size++;
    }

  return 0;
  
}

int move_character(dungeon_t *dungeon, characterQueue_t *characterQueue, int turnNumber)
{
  int i, found, randPos, xPos, yPos, j, validPosition, numRooms, updateX, updateY;
  int corridorCheck, seenInCorridor;
  character_t character, defeatedCharacter;

  i = 0;
  found = 0;
  
  while(found == 0 && i < characterQueue->size)
    {
      character = characterQueue->characterQueue[i];
      
      if(character.nextTurn <= turnNumber)
	{
	  found = 1;
	}

      i++;
    }

  if(found == 1)
    {
      if(character.hero != NULL)
	{

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

	      if(xPos <= 79 && yPos <= 20)
		{
		  validPosition = 1;
		}

	    }
	  for(j = 0; j < characterQueue->size; j++)
	    {
	      //Defeating the monster
	      if(xPos == characterQueue->characterQueue[j].xPos && yPos == characterQueue->characterQueue[j].yPos)
		{
		  characterQueue->characterQueue[j].monster = NULL;
		  characterQueue->characterQueue[j].symbol = NULL;
		  characterQueue->characterQueue[j].xPos = NULL;
		  characterQueue->characterQueue[j].yPos = NULL;
		  characterQueue->characterQueue[j].nextTurn = NULL;
		  characterQueue->characterQueue[j].speed = NULL;
		}
	    }

	  //If moving into a wall
	  if(dungeon->dungeonArray[yPos][xPos].hardness != 0)
	    {
	      dungeon->dungeonArray[yPos][xPos].hardness = 0;
	      dungeon->dungeonArray[yPos][xPos].symbol = '#';
	    }

	  character.nextTurn = character.nextTurn + (1000 / character.speed);
	  

	  characterQueue->characterQueue[i] = character;
	}

      else
	{
	  if((int)character.monster->traits >> 3 == 1 && (rand() % 2) == 1)
	    {
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

		  if(xPos <= 79 && yPos <= 20 && ((int)(character.monster->traits >> 2) % 2 == 1 && dungeon->dungeonArray[yPos][xPos].hardness == 0))
		    {
		      validPosition = 1;
		    }
		}

	      character.xPos = xPos;
	      character.yPos = yPos;
	    }

	  else if((int)(character.monster->traits >> 1) % 2 == 1)
	    {
	      if((int)(character.monster->traits >> 2) % 2 == 1)
		{
		  xPos = dungeon->vertexTunnelMap[character.yPos][character.xPos]->prev->xPos;
		  yPos = dungeon->vertexTunnelMap[character.yPos][character.xPos]->prev->yPos;
		}

	      else
		{
		  xPos = dungeon->vertexMap[character.yPos][character.xPos]->prev->xPos;
		  yPos = dungeon->vertexMap[character.yPos][character.xPos]->prev->yPos;
		}

	      character.xPos = xPos;
	      character.yPos = yPos;
	    }

	  else if((character.monster->traits % 2 == 1))
	    {
	      for(numRooms = 0; numRooms < dungeon->numRooms; numRooms++)
		{
		  //Testing to see if the monster is in the same room as the hero
		  if((character.xPos >= dungeon->roomArray[numRooms].xPos && character.xPos <= dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize && character.yPos >= dungeon->roomArray[numRooms].yPos && character.yPos <= dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize) && (characterQueue->characterQueue[0].xPos >= dungeon->roomArray[numRooms].xPos && characterQueue->characterQueue[0].xPos <= dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize && characterQueue->characterQueue[0].yPos >= dungeon->roomArray[numRooms].yPos && characterQueue->characterQueue[0].yPos <= dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize))
		    {
		      //Updating monster's memory
		      if((int)character.monster->traits >> 2 % 2 == 1)
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster->memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				}
			    }
			}

		      else
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster->memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
				}
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
		      //Updating monster's memory
		      if((int)character.monster->traits >> 2 % 2 == 1)
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster->memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				}
			    }
			}

		      else
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster->memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
				}
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
			  //Updating monster's memory
			  if((int)character.monster->traits >> 2 % 2 == 1)
			    {
			      for(updateY = 0; updateY < 21; updateY++)
				{
				  for(updateX = 0; updateX < 80; updateX++)
				    {
				      character.monster->memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				    }
				}
			    }

			  else
			    {
			      for(updateY = 0; updateY < 21; updateY++)
				{
				  for(updateX = 0; updateX < 80; updateX++)
				    {
				      character.monster->memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
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
		      if((int)character.monster->traits >> 2 % 2 == 1)
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster->memory[updateY][updateX] = dungeon->vertexTunnelMap[updateY][updateX];
				}
			    }
			}

		      else
			{
			  for(updateY = 0; updateY < 21; updateY++)
			    {
			      for(updateX = 0; updateX < 80; updateX++)
				{
				  character.monster->memory[updateY][updateX] = dungeon->vertexMap[updateY][updateX];
				}
			    }
			}
		    }
		}

	      //If they don't see the hero, they stay dormant
	      if(character.monster->memory[yPos][xPos]->prev == NULL)
		{
		  character.xPos = character.xPos;
		  character.yPos = character.yPos;
		}

	      //Otherwise they take the fastest route
	      else
		{
		  character.xPos = character.monster->memory[yPos][xPos]->prev->xPos;
		  character.yPos = character.monster->memory[yPos][xPos]->prev->yPos;
		}
	    }

	    //Else if the monster is only unintelligent
	    else
	      {
		for(numRooms = 0; numRooms < dungeon->numRooms; numRooms++)
		{
		  //Testing to see if the monster is in the same room as the hero
		  if((character.xPos >= dungeon->roomArray[numRooms].xPos && character.xPos <= dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize && character.yPos >= dungeon->roomArray[numRooms].yPos && character.yPos <= dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize) && (characterQueue->characterQueue[0].xPos >= dungeon->roomArray[numRooms].xPos && characterQueue->characterQueue[0].xPos <= dungeon->roomArray[numRooms].xPos + dungeon->roomArray[numRooms].xSize && characterQueue->characterQueue[0].yPos >= dungeon->roomArray[numRooms].yPos && characterQueue->characterQueue[0].yPos <= dungeon->roomArray[numRooms].yPos + dungeon->roomArray[numRooms].ySize))
		    {
		      //Move towards hero
		      if((int)character.monster->traits >> 2 % 2 == 1)
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
		      if((int)character.monster->traits >> 2 % 2 == 1)
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
			  if((int)character.monster->traits >> 2 % 2 == 1)
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
		      if((int)character.monster->traits >> 2 % 2 == 1)
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
		//Checking to see if the monster is moving into and defeating a hero
		if(character.xPos == characterQueue->characterQueue[j].xPos && character.yPos == characterQueue->characterQueue[j].yPos)
		  {
		    characterQueue->characterQueue[j].hero = NULL;
		    characterQueue->characterQueue[j].monster = NULL;
		    characterQueue->characterQueue[j].symbol = NULL;
		    characterQueue->characterQueue[j].xPos = NULL;
		    characterQueue->characterQueue[j].yPos = NULL;
		    characterQueue->characterQueue[j].nextTurn = NULL;
		    characterQueue->characterQueue[j].speed = NULL;
		  }
	      }

	    //If moving through a wall
	    if(dungeon->dungeonArray[yPos][xPos].hardness != 0)
	      {
		dungeon->dungeonArray[yPos][xPos].hardness = 0;
		dungeon->dungeonArray[yPos][xPos].symbol = '#';
	      }

	    character.nextTurn = character.nextTurn + (1000 / character.speed);
	  

	    characterQueue->characterQueue[i] = character;
	}
    }
  return turnNumber + 1;
}


int check_win_condition(characterQueue_t *characterQueue)
{
  int i;

  if(characterQueue->characterQueue[0].hero == NULL)
    {
      return -1;
    }

  else
    {
      for(i = 1; i < characterQueue->size; i++)
	{
	  if(characterQueue->characterQueue[i].monster != NULL)
	    {
	      return 0;
	    }
	}

      return 1;
    }
}

*/
