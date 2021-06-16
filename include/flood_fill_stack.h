/*
 * Created by lowen.desmarais on 05/04/2021.
 *
 * File containing all prototypes for the stack impleemntation.
 * It is mainly used for the flood fill algorithm
 * Code has been heavily influenced by online courses
 */

#ifndef STACK_H
#define STACK_H

#include<stdlib.h>

struct ff_coord
{
    size_t x1;
    size_t x2;
    size_t y;
    char dy;
};

struct coord
{
  size_t x;
  size_t y;
};

typedef  struct ff_stack
{
    size_t capacity;
    size_t size;
    struct ff_coord *data;
}ff_stack;

ff_stack* new_stack();

void double_capacity(ff_stack *s);

void stack_push(ff_stack *s, struct ff_coord node);

struct ff_coord stack_pop(ff_stack *s);

int stack_IsEmpty(ff_stack *s);

void stack_free(struct ff_stack* s);

#endif
