/*
 * Created by titalah on 05/04/2021.
 * stack with vector implementation code, derived from EPITA
*/

#include "stack.h"
#include <err.h>

stack *new_stack()
{
    struct stack *s = malloc(sizeof(struct stack));
    if (s == NULL)
        errx(EXIT_FAILURE, "new_stack: Memory Error");

    s->capacity = 1;
    s->size = 0;
    s->data = malloc(s->capacity * sizeof(struct coord));

    if (s->data == NULL)
        errx(EXIT_FAILURE, "new_stack: data Memory Error");

    return s;
}

void double_capacity(stack *s)
{
    s->capacity *= 2;
    if((s->data = realloc(s->data, s->capacity * sizeof(struct coord))) == NULL)
        errx(EXIT_FAILURE, "double_capacity: Memory Error");
}

void stack_push(stack *s, struct coord node)
{
    if (s->size == s->capacity)
        double_capacity(s);

    s->data[s->size] = node;
    s->size += 1;
}

struct coord stack_pop(stack *s)
{
    s->size -= 1;
    return s->data[s->size];
}

int stack_IsEmpty(stack *s)
{
    return s == NULL || !s->size;
}

void stack_free(struct stack* s)
{
    free(s->data);
    free(s);
}