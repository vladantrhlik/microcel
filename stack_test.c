#include "stack.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	/* testing stack */
	stack *s = stack_init();
	if (!s) return 1;

	char buff[32];
	
	while (1) {
		printf(": ");
		scanf("%s", buff);

		if (!strcmp(buff, "pop")) {
			char *popped = stack_pop(s);
			if (popped) {
				printf("'%s'\n", popped);
				free(popped);
			} else printf("nothing in stack\n");
		} else if(!strcmp(buff, "exit")) {
			stack_free(&s);
			return 0;
		} else if (!strcmp(buff, "revert")) {
			stack_revert(s);	
		} else {
			char *new = malloc(sizeof(char) * (strlen(buff)+1));
			if (!new) return 1;

			strcpy(new, buff);

			if (!stack_push(s, new)) {
				printf("'%s' added to stack\n", new);
			} else printf("failed to add new entry\n");
		}
	}

	return 0;
}
