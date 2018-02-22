#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "priorityqueue.h"
#include "dijkstra.h"


typedef int bool;
#define true 1
#define false 0

/*This is the method to perform dijkstra's algorithm on the vertexArray. There is some code here from
  priorityQueue because I was having trouble integrating priorityQueue's functions into dijkstra */
int dijkstra(vertex_t* vertexArray, int arraySize)
{
  int i, j, altDistance, minRemove, sort;
  priorityQueue_t priorityQueue;
  vertex_t minVertex;
  vertex_t neighbor;
  vertex_t vertexAdded;
  bool sorted = false;
  bool isEmpty = false;

  priorityQueue.size = 0;
  priorityQueue.vertexArray = (vertex_t*)calloc(arraySize, sizeof(vertex_t));

  //Adding the vertices from the vertexArray into the priorityQueue
  for(i = 0; i < arraySize; i++)
    {

      //Added add_vertex code here
      sorted = false;

      vertexArray[i].priority = vertexArray[i].distance;
      vertexArray[i].visited = 0;

      vertexAdded = vertexArray[i];

      if(priorityQueue.size >=1)
	{

	  //Sorts the priorityQueue based on priority. Highest priority is the last in the array
	  sort = priorityQueue.size - 1;
	  while(!sorted && sort > -1)
	    {
	      if(priorityQueue.vertexArray[sort].priority < vertexAdded.priority)
		{
		  priorityQueue.vertexArray[sort+1] = priorityQueue.vertexArray[sort];
		  priorityQueue.vertexArray[sort] = vertexAdded;
		  sort--;
		}

	      else
		{
		  priorityQueue.vertexArray[sort + 1] = vertexAdded;
		  sorted = true;
		}
	    }
	}

      else
	{
	  priorityQueue.vertexArray[priorityQueue.size] = vertexAdded;
	}
      //add_vertex(&vertexArray[i], vertexArray[i].distance, priorityQueue.size, &priorityQueue);

      priorityQueue.size++;
    }


  //Dijkstra's Algorithm
  while(is_empty(&priorityQueue) == 0)
    {
      altDistance = 0;

      //remove_min code here

      minRemove = priorityQueue.size - 1;
      while(priorityQueue.vertexArray[minRemove].visited == 1)
	{
	  minRemove--;
	}

      minVertex = priorityQueue.vertexArray[minRemove];
      priorityQueue.vertexArray[minRemove].visited = 1;


      //remove_min(&priorityQueue, &minVertex);

      minVertex.visited = 1;
      
      //printf("Checking neighbor 1\n");
      //Checks the neighbor 1 above the vertex
      if(minVertex.xPos < 79 && minVertex.xPos > 0 && (minVertex.yPos - 1) < 20 && (minVertex.yPos - 1) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{
	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos && priorityQueue.vertexArray[i].yPos == minVertex.yPos - 1)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;

	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}		 
	    }
	}

      //printf("Checking neighbor 2\n");
      //Checks the neighbor 1 above and 1 to the right of the vertex
      if((minVertex.xPos + 1) < 79 && (minVertex.xPos + 1) > 0 && (minVertex.yPos - 1) < 20 && (minVertex.yPos - 1) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{
	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos + 1 && priorityQueue.vertexArray[i].yPos == minVertex.yPos - 1)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;

	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}		  
	    }
	}

      //printf("Checking neighbor 3\n");
      //Checks the neighbor 1 to the right of the vertex
      if((minVertex.xPos + 1) < 79 && (minVertex.xPos + 1) > 0 && (minVertex.yPos) < 20 && (minVertex.yPos) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{
	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos + 1 && priorityQueue.vertexArray[i].yPos == minVertex.yPos)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;


	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}		  
	    }
	}

      //printf("Checking neighbor 4\n");
      //Checks the neighbor 1 to the right and 1 down of the vertex
      if((minVertex.xPos + 1) < 79 && (minVertex.xPos + 1) > 0 && (minVertex.yPos + 1) < 20 && (minVertex.yPos + 1) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{
	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos + 1 && priorityQueue.vertexArray[i].yPos == minVertex.yPos + 1)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;


	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}		  
	    }
	}

      //printf("Checking neighbor 5\n");
      //Checks the neighbor 1 down of the vertex
      if((minVertex.xPos) < 79 && (minVertex.xPos) > 0 && (minVertex.yPos + 1) < 20 && (minVertex.yPos + 1) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{

	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos && priorityQueue.vertexArray[i].yPos == minVertex.yPos + 1)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;

	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}		  
	    }
	}
      //printf("Checking neighbor 6\n");
      //Checks the neighbor 1 to the left and 1 down of the vertex
      if((minVertex.xPos - 1) < 79 && (minVertex.xPos - 1) > 0 && (minVertex.yPos + 1) < 20 && (minVertex.yPos + 1) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{

	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos - 1 && priorityQueue.vertexArray[i].yPos == minVertex.yPos + 1)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;

	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}
	    }
	}

      //printf("Checking neighbor 7\n");
      //Checks the neighbor 1 to the left of the vertex
      if((minVertex.xPos - 1) <= 79 && (minVertex.xPos - 1) > 0 && (minVertex.yPos) < 20 && (minVertex.yPos) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{

	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos - 1 && priorityQueue.vertexArray[i].yPos == minVertex.yPos)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;

	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}
	    }
	}

      // printf("Checking neighbor 8\n");
      //Checks the neighbor 1 to the left and 1 up of the vertex
      if((minVertex.xPos - 1) < 79 && (minVertex.xPos - 1) > 0 && (minVertex.yPos - 1) < 20 && (minVertex.yPos - 1) > 0 && minVertex.xPos < 79 && minVertex.xPos > 0 && minVertex.yPos < 20 && minVertex.yPos > 0)
	{
	  for(i = 0; i < priorityQueue.size; i++)
	    {
	      if(priorityQueue.vertexArray[i].xPos == minVertex.xPos - 1 && priorityQueue.vertexArray[i].yPos == minVertex.yPos - 1)
		{
		  neighbor = priorityQueue.vertexArray[i];
		}
	    }

	  altDistance = minVertex.distance + neighbor.weight;

	  if(neighbor.weight == 255)
	    {
	      //Do nothing
	    }

	  else if(altDistance < neighbor.distance)
	    {
	      neighbor.distance = altDistance;
	      neighbor.prev = &minVertex;
	      neighbor.priority = altDistance;

	      sorted = false;
	      i = 0;

	      //decrease_priority code
	      while(!sorted)
		{
		  if(priorityQueue.vertexArray[i].xPos == neighbor.xPos && priorityQueue.vertexArray[i].yPos == neighbor.yPos)
		    {
		      priorityQueue.vertexArray[i] = neighbor;
		      sort = i;
		      while(!sorted && sort < priorityQueue.size)
			{
			  if(priorityQueue.vertexArray[sort].priority < priorityQueue.vertexArray[sort + 1].priority)
			    {
			      priorityQueue.vertexArray[sort] = priorityQueue.vertexArray[sort + 1];
			      priorityQueue.vertexArray[sort + 1] = neighbor;
			      sort++;
			    }

			  else
			    {
			      sorted = true;
			    }
			}
		    }
		  i++;

		}
	    }
	}

    }

  //Updating vertexArray based on the newly dijkstrated priorityQueue
  for(i = 0; i < priorityQueue.size; i++)
    {
      for(j = 0; j < priorityQueue.size; j++)
	{
	  if(priorityQueue.vertexArray[i].xPos == vertexArray[j].xPos && priorityQueue.vertexArray[i].yPos == vertexArray[j].yPos)
	    {
	      vertexArray[j] = priorityQueue.vertexArray[i];
	    }
	}
    }

  free(priorityQueue.vertexArray);
  return 0;
}
