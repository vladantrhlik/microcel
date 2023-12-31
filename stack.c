#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define SIZE_STEP 10

stack *stack_init() {
	stack *s = malloc(sizeof(stack));
	if (!s) return NULL;
	
	s->count = 0; 
	s->size = SIZE_STEP;
	s->data = malloc(SIZE_STEP * sizeof(void *));

	if (!s->data) {
		free(s);
		return NULL;
	}

	return s;
}

int stack_push(stack *s, void *data) {
	if (!data || !s) return -1;

	/* check if there's enough space for new entry */
	if (s->count+1 > s->size) {
		s->data = realloc(s->data, s->size + SIZE_STEP);
		s->size += SIZE_STEP;
	}

	s->data[s->count] = data;
	s->count++;
	
	return 0;
}

void *stack_pop(stack *s) {
	if (!s) return NULL;
	
	if (s->count <= 0) return NULL;
	
	s->count--;
	return s->data[s->count];
}

void stack_free(stack **s) {
	if (!s || !*s) return;
	free(*s);
	*s = NULL;
}
