#include "adt/list.h"
#include "adt/stack.h"
#include "tokenizer.h"
#include <stdio.h>

int opprec(token *t) {
	if (t->type != TT_BOP) return 0;

	switch (t->ch) {
		case '+':
		case '-':
			return 2;
		case '/':
		case '*':
			return 3;
		default:
			printf("Undefined precedence for '%c' operator\n", t->ch);
			return -1;
	}
}

list *postfix(list *tokens) {
	if (!tokens) return NULL;

	list *out = list_init();
	stack *opstack = stack_init();

	for (int i = 0; i<tokens->count; i++) {
		printf("token %d\n", i);
		token *t = (token *) list_get(tokens, i);
		token *o = NULL; /* used for top of op stack */

		switch (t->type) {
			case TT_INT:
			case TT_FLOAT:
				list_add(out, t);
				break;
			case TT_LIT:
				/* TODO replace constants with int or float tokens */
				printf("Unknown const: '%s'\n", t->lit);
				return NULL;
				break;
			case TT_BOP:
				while ( (o = (token*) stack_peak(opstack)) && o->type != TT_OPAR && opprec(o) > opprec(t) /* TODO asociativity */) {
					list_add(out, stack_pop(opstack));	
				}
				stack_push(opstack, t);
				break;
			case TT_OPAR:
				stack_push(opstack, t);
				break;
			case TT_CPAR:
				while ( (o = (token*) stack_peak(opstack)) && o->type != TT_OPAR) {
					list_add(out, stack_pop(opstack));	
				}
				if (!o) {
					printf("Missmatched parentheses\n");
					return NULL;
				}
				/* pop '(' from op stack */
				stack_pop(opstack);
				/* TODO: function on top of op stack */
				break;
			case TT_EMPTY: break;
		}
	}

	while (stack_peak(opstack)) {
		list_add(out, stack_pop(opstack));
	}

	return out;
}
