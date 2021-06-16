/*
 * Created by titalah on 05/04/2021.
 * stack with vector implementation code, derived from EPITA
*/

#include "../../include/flood_fill_stack.h"
#include <err.h>

ff_stack *f_new_stack()
{
    struct ff_stack *s = malloc(sizeof(struct ff_stack));
    if (s == NULL)
        errx(EXIT_FAILURE, "new_stack: Memory Error");

    s->capacity = 1;
    s->size = 0;
    s->data = malloc(s->capacity * sizeof(struct ff_coord));

    if (s->data == NULL)
        errx(EXIT_FAILURE, "new_stack: data Memory Error");

    return s;
}

void f_double_capacity(ff_stack *s)
{
    s->capacity *= 2;
    if((s->data = realloc(s->data, s->capacity * sizeof(struct ff_coord))) == NULL)
        errx(EXIT_FAILURE, "double_capacity: Memory Error");
}

void f_stack_push(ff_stack *s, struct ff_coord node)
{
    if (s->size == s->capacity)
        f_double_capacity(s);

    s->data[s->size] = node;
    s->size += 1;
}

struct ff_coord f_stack_pop(ff_stack *s)
{
    s->size -= 1;
    return s->data[s->size];
}

int f_stack_IsEmpty(ff_stack *s)
{
    return s == NULL || !s->size;
}

void f_stack_free(struct ff_stack* s)
{
    free(s->data);
    free(s);
}
