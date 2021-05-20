#include "../../include/utils.h"

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

<<<<<<< HEAD
=======
/**
 * Pop data on the top of a stack_ptr
 * @param stack_ptr pointer to the top of the stack
 */
void *pop_from_stack_at_pos(Stack **stack_ptr,int pos){
    Stack *front = *stack_ptr;
    void *data = NULL;
    if (!is_stack_empty(*stack_ptr)) {
	if (pos == 0){
        	*stack_ptr = front->next;
        	data = front->data;
		return data;
	}
 	for(;pos-1>0;pos--){
		front = front->next;
	}
	data = front->data;
	front->next = front->next->next;
    }
    return data;
}

/**
 * return data on the pos of a stack_ptr
 * @param stack_ptr pointer to the top of the stack
 */
void *elm_at_pos(Stack **stack_ptr,int pos){
    Stack *front = *stack_ptr;
    void *data = NULL;
    if (!is_stack_empty(*stack_ptr)) {
	
 	for(;pos>0;pos--)
		front = front->next;
	data = front->data;
    }
    return data;
}

>>>>>>> a45353351d90160d036ed14ea417d7152378332e
void swap_next_el(Stack **stack_ptr,int pos){ 
	Stack *actual = *stack_ptr;
	if (pos == 0){
		Stack *tmpend = actual->next;
		Stack *tmp = actual->next->next;
		actual->next->next = actual;
		actual->next = tmp;
		*stack_ptr = tmpend;
		return;
	}
	
	for(;pos-1>0;pos--){
		actual = actual->next;
	}
	Stack *tmpel4 = actual->next->next->next;
	Stack *tmpel3 = actual->next->next;

	actual->next->next->next = actual->next;
	actual->next = tmpel3;
	actual->next->next->next = tmpel4; 
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
