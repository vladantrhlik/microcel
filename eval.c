#include "adt/list.h"
#include "adt/stack.h"
#include "tokenizer.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

int opprec(token *t) {
	if (t->type != TT_BOP) return 0;

	switch (t->ch) {
		case '^':
			return 4;
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
			case TT_FUNC:
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

int evalpostfix(list *tokens, float *res) {
	if (!tokens) return -1;

	token *op1, *op2, *new;
	float result;
	
	stack *s = stack_init();
	if (!s) return -1;

	for (int i = 0; i<tokens->count; i++) {
		token *t = (token*) list_get(tokens, i);
		switch (t->type) {
			case TT_INT:
				/* convert to float token */
				t->type = TT_FLOAT;
				t->fnum = (float) t->inum;
				stack_push(s, t);
				break;
			case TT_FLOAT:
				stack_push(s, t);
				break;
			case TT_LIT:
				printf("Found literal while evaluating\n");
				return -1;
			case TT_CPAR:
			case TT_OPAR:
				printf("Found parenthese while evaluating\n");
				return -1;
			case TT_FUNC:
				result = 0;
				op1 = (token*) stack_pop(s);
				
				if (!op1) return -1;

				if (!strcmp(t->lit, "sin")) {
					result = sin((double) op1->fnum);
				} else if (!strcmp(t->lit, "cos")) {
					result = cos((double) op1->fnum);
				} else {
					printf("Undefined function: %s\n", t->lit);
					return -1;
				}

				/* push result to stack */
				new = token_init(TT_FLOAT);
				new->fnum = result;
				stack_push(s, new);
				break;
			case TT_BOP:
				/* get operands and convert them to float */
				op2 = (token*) stack_pop(s);
				op1 = (token*) stack_pop(s);

				if (!op2 || !op1) return -1;
			
				float val1 = op1->fnum;
				float val2 = op2->fnum;

				result = 0;

				switch (t->ch) {
					case '+':
						result = val1 + val2;
						break;
					case '-':
						result = val1 - val2;
						break;
					case '*':
						result = val1 * val2;
						break;
					case '/':
						result = val1 / val2;
						break;
					case '^':
						result = (float) pow((double) val1, (double) val2);
				}
				
				/* push result to stack */
				new = token_init(TT_FLOAT);
				new->fnum = result;

				stack_push(s, new);
				break;
		}
	}
	
	token *out = stack_pop(s);
	if (out && out->type == TT_FLOAT) {
		*res = out->fnum;
	} else {
		return -1;
	}

	return 0;
}

int eval(list *tokens, float *res) {
	if (!tokens) return -1;
	
	/* convert to postfix notation */
	list *p = postfix(tokens);
	if (!p) {
		printf("Error while converting infix to postfix\n");
		return -1;
	}
	
	/* evaluate postfix */
	if (evalpostfix(p, res) == -1) {
		printf("Error while evaluating postfix notation\n");	
		return -1;
	}

	return 0;
}
