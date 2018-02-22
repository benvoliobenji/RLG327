#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "priorityqueue.h"

typedef int bool;
#define true 1
#define false 0


/*Adding a vertex into the priorityQueue */
int add_vertex(vertex_t* vertex, int priority, int size, priorityQueue_t* priorityQueue)
{
  bool sorted = false;
  int i;

  printf("%d\n", priority);
  printf("Adding vertex\n");
  
  vertex->priority = priority;
  vertex->visited = 0;

  printf("Reallocating priorityQueue\n");

  printf("%d\n", size);

  //priorityQueue->size = priorityQueue->size + 1;

  //printf("%d\n", priorityQueue->size);
  //priorityQueue->vertexArray = realloc(priorityQueue->vertexArray, (priorityQueue->size) * sizeof(vertex_t));

  printf("Adding vertex to vertexArray\n");

  priorityQueue->vertexArray[(size)-1] = *vertex;

  if(size >=2)
    {
      printf("priorityQueue has size >=2\n");

      i = size - 2;
      while(!sorted && i >=0)
	{
	  printf("sorting based on priority\n");
	  if(priorityQueue->vertexArray[i].priority < vertex->priority)
	    {
	      priorityQueue->vertexArray[i+1] = priorityQueue->vertexArray[i];
	      priorityQueue->vertexArray[i] = *vertex;
	      i--;
	    }

	  else
	    {
	      sorted = true;
	    }
	}
    }

  return 0;
  
}

int remove_min(priorityQueue_t* priorityQueue, vertex_t* min_vertex)
{
  int i = priorityQueue->size - 1;
  
  while(priorityQueue->vertexArray[i].visited == 1)
    {
      i--;
    }

  min_vertex = &priorityQueue->vertexArray[i];
  priorityQueue->vertexArray[i].visited = 1;

  return 0;
  
}

/*Given a vertex, a priority, and a priorityQueue, searches for the correllating vertex in the priority queue, updates the priority, and sorts the queue */ 
int decrease_priority(priorityQueue_t* priorityQueue, vertex_t vertex, int priority)
{
  int i;
  for(i = 0; i < priorityQueue->size; i++)
    {
      if(priorityQueue->vertexArray[i].xPos == vertex.xPos && priorityQueue->vertexArray[i].yPos == vertex.yPos)
	{
	  priorityQueue->vertexArray[i].priority = priority;
	}
    }

  return 0;
}

/*Returns if the priorityQueue is empty or not */
int is_empty(priorityQueue_t* priorityQueue)
{
  int i;

  for(i = 0; i < priorityQueue->size; i++)
    {
      if(priorityQueue->vertexArray[i].visited == 0)
	{
	  return 0;
	}
    }

  return 1;
}

/*Given the priorityQueue, a x and y position, finds the vertex in the priorityQueue and sets it to the foundVertex if it is within legal bounds */
int find_vertex(priorityQueue_t* priorityQueue, vertex_t* foundVertex, int xPos, int yPos)
{
  int i;

  if(xPos >= 79 || yPos >= 20 || xPos <= -1 || yPos <= -1)
    {
      return -1;
    }

  for(i = 0; i < priorityQueue->size; i++)
    {
      if(priorityQueue->vertexArray[i].xPos == xPos && priorityQueue->vertexArray[i].yPos == yPos)
	{
	  foundVertex = &priorityQueue->vertexArray[i];
	  return 0;
	}
    }

  return -1;
}

