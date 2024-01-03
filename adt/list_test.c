#include <stdio.h>
#include "list.h"

int main(int argc, char *argv[]) {
	list *l = list_init();

	list_add(l, "bob");
	list_add(l, "dod");
	list_add(l, "adam");

	for (int i = 0; i<l->count; i++) {
		printf("%d: %s\n", i, list_get(l, i));
	}

	list_insert(l, "test", 1);

	for (int i = 0; i<l->count; i++) {
		printf("%d: %s\n", i, list_get(l, i));
	}


	return 0;
}
