#include "analyzer.h"
#include "tokenizer.h"
#include "adt/list.h"
#include <stdio.h>

/**
* Problematic cases to fix:
*	- '-6*5' => make 6 negative
*	- '-(42/3)' => make it '0-(42/3)'
*	- 43*-(5/3)
* Wrong expression:
*	- two operators except '<op>-' are not ok
*	- two literals after another is not ok
*/

int analyze(list *tokens) {
	if (!tokens) return -1;

	/* fix '+'/'-' at the start of expression */
	token *first = list_get(tokens, 0);
	if (first && first->type == TT_BOP && (first->ch == '+' || first->ch == '-')) {
		token *zero = token_init(TT_INT);
		zero->inum = 0;
		list_insert(tokens, zero, 0);
	}

	for (int i = 0; i<tokens->count-1; i++) {
		token *prev = i >= 1 ? (token *) list_get(tokens, i-1) : NULL;
		token *cur = (token *) list_get(tokens, i);
		token *next = (token *) list_get(tokens, i+1);

		/* two literals in row */ 
		if (isvalue(cur->type) && isvalue(next->type)) {
			printf("Wrong expression syntax. (two literals after another)\n");
			return -1;
		}

		if (cur->type == TT_BOP && (cur->ch == '-' || cur->ch == '+') ) {
			/* '-/+' before literal */
			if (prev && !isvalue(prev->type) && isvalue(next->type)) {
				if (cur->ch == '-') {
					/* negate next token value */
					if (next->type == TT_INT) next->inum *= -1;
					else if (next->type == TT_FLOAT) next->fnum *= -1;
					else printf("Negation before string literal\n");
				}
				/* remove operation token */
				list_remove(tokens, i);
			}
			/* '<bop>+/-(...' */
			if (prev && prev->type == TT_BOP && next->type == TT_OPAR) {
				/* change +/- to 1 */
				cur->type = TT_INT;
				cur->inum = cur->ch == '-' ? -1 : 1;
				/* add '*' symbol between '1' and '(' */
				token *mul = token_init(TT_BOP);
				mul->ch = '*';
				list_insert(tokens, mul, i+1);
			}
		}



	}
	return 0;
}
