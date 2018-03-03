#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
typedef int bool;
#define true 1
#define false 0
*/

typedef struct vertex{
  int distance;
  struct vertex *prev;
  int xPos;
  int yPos;
  int visited;
  int priority;
  int weight;
} vertex_t;

typedef struct priorityQueue{
  int size;
  vertex_t* vertexArray;
}priorityQueue_t;

int add_vertex(vertex_t* vertex, int priority, int size, priorityQueue_t* priorityQueue);
int remove_min(priorityQueue_t* priorityQueue, vertex_t* min_vertex);
int decrease_priority(priorityQueue_t* priorityQueue, vertex_t vertex, int priority);
int is_empty(priorityQueue_t* priorityQueue);
int find_vertex(priorityQueue_t* priorityQueue, vertex_t* foundVertex, int xPos, int yPos);

#endif
