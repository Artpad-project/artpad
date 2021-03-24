#include "utils.h"

/**
 * Initialize an empty stack
 * @return the sentinel
 */
Stack *create_stack() {
    Stack *stack = malloc(sizeof (Stack));
    stack->next = NULL;
    stack->data = NULL;
    return stack;
}

/**
 * Push data onto a stack_ptr
 * @param stack_ptr     top of the stack
 * @param data          data casted to void pointer
 * @returns             top of the new stack
 */
Stack *push_to_stack(Stack *stack_ptr, void *data) {
    Stack *new_node = create_stack();
    new_node->data = data;
    new_node->next = stack_ptr;

    return new_node;
}

/**
 * Pop data on the top of a stack_ptr
 * @param stack_ptr pointer to the top of the stack
 */
void *pop_from_stack(Stack **stack_ptr){
    Stack *front = *stack_ptr;
    void *data = NULL;
    if (!is_stack_empty(*stack_ptr)) {
        *stack_ptr = front->next;
        data = front->data;
    }
    return data;
}

/**
 * Check if a stack is empty
 * @param stack
 * @return 1    if empty else 0
 */
int is_stack_empty(Stack *stack) {
    ASSERT(stack != NULL, 1);
    return stack->next == NULL;
}

void free_stack(Stack *stack) {
    Stack *stack_to_free = NULL;
    while (stack->next != NULL) {
        stack_to_free = stack;
        stack = stack->next;
        free(stack_to_free->data);
        free(stack_to_free);
    }
}
