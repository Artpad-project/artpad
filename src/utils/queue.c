#include <err.h>
#include <stdlib.h>
#include "../../include/queue.h"

queue* queue_push(queue* start, Image img)
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

queue* queue_pop(queue* start, Image *img)
{
  if (start == NULL)
    return NULL;

  queue *q = start->next;
  *img = q->img;
  if (q->next == q)
  {
    free(q);
    return NULL;
  }

  start->next = q->next;
  free(q);
  return start;
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
