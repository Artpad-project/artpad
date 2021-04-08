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

struct coord
{
    size_t x;
    size_t y;
};

typedef  struct stack
{
    size_t capacity;
    size_t size;
    struct coord *data;
}stack;

stack* new_stack();

void double_capacity(stack *s);

void stack_push(stack *s, struct coord node);

struct coord stack_pop(stack *s);

int stack_IsEmpty(stack *s);

void stack_free(struct stack* s);

#endif