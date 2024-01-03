#ifndef STACK
#define STACK

#define STACK_SIZE_STEP 10

typedef struct {
	int count; /* number of entries */
	int size; /* current size of buffer */
	void **data; 
} stack;

/**
* Inits stack structure with default capacity
*/
stack *stack_init();

/**
* Pushes new data to stack
* @param stack *s stack structure to push data to
* @param void *data new data
*/
int stack_push(stack *s, void *data);

/**
* Returns value from top of stack
*/
void *stack_pop(stack *s);

/**
 * Reverse order of items in stack :D
 */
void stack_revert(stack *s);

/**
* Frees stack structure but not its content
*/
void stack_free(stack **s);

#endif
