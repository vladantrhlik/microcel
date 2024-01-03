#include <stdlib.h>
#include "list.h"

list *list_init() {
	list *l = malloc(sizeof(list));
	if (!l) return NULL;
	
	l->count = 0;
	l->size = LIST_SIZE_STEP;
	l->data = malloc(sizeof(void *) * l->size);

	if (!l->data) {
		free(l);
		return NULL;
	}

	return l;
}

int list_add(list *l, void *data) {
	if (!l || !data) return -1;

	/* check if there's space */
	if (l->count+1 > l->size) {
		l->size += LIST_SIZE_STEP;
		l->data = realloc(l->data, l->size);
	}
	
	/* add data */
	l->data[l->count] = data;
	l->count++;

	return 0;
}

void *list_get(list *l, int index) {
	if (!l) return NULL;

	if (index >= 0 && index < l->count) {
		return l->data[index];
	}

	return NULL;
}

int list_remove(list *l, int index) {
	if (!l) return -1;

	if (index >= 0 && index < l->count) {
		for (int i = index+1; i<l->count; i++) {
			l->data[i-1] = l->data[i];	
		}
		l->count--;
		return 0;
	}

	return -1;
}

int list_insert(list *l, void *data, int index) {
	if (!l || !data) return -1;
	
	if (index >= 0 && index < l->count) {
		/* add new data to potentially realloc */
		list_add(l, data);
		for (int i = l->count-2; i>=index; i--) {
			l->data[i+1] = l->data[i];	
		}
		l->data[index] = data;
		return 0;
	}

	return -1;
}
