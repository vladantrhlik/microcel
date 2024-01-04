#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

int main(int argc, char *argv[]) {
	list *l = list_init();

	char buf[32];

	while (1) {
		printf(": ");
		scanf("%s", buf);

		char *n = malloc(sizeof(char) * strlen(buf));
		strcpy(n, buf);

		list_add(l, n);

		/* print list contents */
		for (int i = 0; i<l->count; i++) {
			printf("%s, ", (char*) l->data[i]);
		}
		printf("\n");
	}

	return 0;
}
