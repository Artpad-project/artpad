#include <err.h>
#include <stdlib.h>
#include "../../include/queue.h"

queue* queue_push(queue* start, Image *img)
{
  queue* q = malloc(sizeof(queue));
  if (q == NULL)
    errx(1, "queue.c: couldn't create new queue");

  q->img = img;
  if (start == NULL)
  {
    q->next = q;
  }
  else 
  {
    q->next = start->next;
    start->next = q;
  }
  return q;
}

//dont forget, here it functions as a stack
queue* queue_pop(queue* start, Image *img)
{
  if (start == NULL)
    return NULL;

  queue *q = start->next;
  while (q->next != start)
    q = q->next;

  img = start->img;
  if (q->next == q)
  {
    free_image(q->img);
    free(q);
    return NULL;
  }

  q->next = start->next;
  free_image(start->img);
  free(start);
  return q;
}

void queue_empty(queue** pstart)
{
  if (pstart == NULL)
    errx(1, "Error: pstart is NULL\n");

  queue* start = *pstart;
  while (start)
    start = queue_pop(start, NULL);

  *pstart = NULL;
}
