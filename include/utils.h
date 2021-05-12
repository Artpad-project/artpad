#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>

struct Stack {
    struct Stack *next;
    void *data;
};

typedef struct Stack Stack;

Stack *create_stack();
Stack *push_to_stack(Stack *stack, void *data);
void *pop_from_stack(Stack **stack);
int is_stack_empty(Stack *stack);

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define SWAP(a,b) ({typeof(a) tmp = a; a = b; b = tmp;})
#define ABS(a) (((a) < 0) ? -(a) : (a))

#ifdef NDEBUG
#define ASSERT(predicate) ((void)0)
#else
#include <stdio.h>
#define __ASSERT_1(predicate) ({if (!(predicate)) __ASSERTION_FAILED(#predicate);})
#define __ASSERTION_FAILED(predicate) \
    ({fprintf(stderr, "assertion_failed:%s:%s:%d - %s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, predicate); return;})

#define __ASSERT_2(predicate, _return) ({if (!(predicate)) __ASSERTION_FAILED_2(#predicate, _return);})
#define __ASSERTION_FAILED_2(predicate, _return) \
    ({fprintf(stderr, "assertion_failed:%s:%s:%d - %s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, predicate); return _return;})

#define __ASSERT_HELPER(_1,_2,NAME,...) NAME
#define ASSERT(...) __ASSERT_HELPER(__VA_ARGS__, __ASSERT_2, __ASSERT_1)(__VA_ARGS__)

#define ASSERT_NOT_REACHED() __ASSERTION_FAILED("Unreachable code reached")
#endif

#endif //UTILS_H_
