#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

stack *stack_init() {
	stack *s = malloc(sizeof(stack));
	if (!s) return NULL;
	
	s->count = 0; 
	s->size = STACK_SIZE_STEP;
	s->data = malloc(STACK_SIZE_STEP * sizeof(void *));

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
		s->size += STACK_SIZE_STEP;
		s->data = realloc(s->data, s->size * sizeof(void*));
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


void *stack_peak(stack *s) {
	if (!s) return NULL;
	
	if (s->count <= 0) return NULL;
	
	return s->data[s->count-1];
}



void stack_revert(stack *s) {
	if (!s) return;

	for (int i = 0; i < s->count/2; i++) {
		int j = s->count-1-i;

		void *tmp = s->data[j];
		s->data[j] = s->data[i];
		s->data[i] = tmp;
	}
}

void stack_free(stack **s) {
	if (!s || !*s) return;
	free(*s);
	*s = NULL;
}
