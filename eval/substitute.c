#include "substitute.h"
#include "../adt/list.h"
#include "tokenizer.h"
#include <string.h>

#define PI 3.141592653589793
#define E 2.718281828459045

int substitute(list *tokens) {
	if (!tokens) return -1;

	for (int i = 0; i<tokens->count; i++) {
		token *t = list_get(tokens, i);
		if (t->type == TT_LIT) {
			if (!strcmp(t->lit, "pi")) {
				t->type = TT_FLOAT;
				t->fnum = PI; 
			} else if (!strcmp(t->lit, "e")) {
				t->type = TT_FLOAT;
				t->fnum = E;
			}
		}
	}

	return 0;
}
