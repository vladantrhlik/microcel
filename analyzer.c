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

	for (int i = 0; i<tokens->count-1; i++) {
		token *cur = (token *) list_get(tokens, i);
		token *next = (token *) list_get(tokens, i+1);

		if (isvalue(cur->type) && isvalue(cur->type)) {
			/* two literals in row */ 
			printf("Wrong expression syntax. (two literals after another)\n");
			return -1;
		}
	}
	return 0;
}
