#ifndef STACK
#define STACK

typedef struct {
	int count; /* number of entries */
	int size; /* current size of buffer */
	void **data; 
} stack;

stack *stack_init();

int stack_push(stack *s, void *data);

void *stack_pop(stack *s);

void stack_free(stack **s);

#endif
