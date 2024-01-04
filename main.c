#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "eval.h"

void printtokens(list *tokens) {
	/* print all tokens */
	for (int i = 0; i<tokens->count; i++) {
		token *t = (token *) tokens->data[i];
		
		switch (t->type) {
			case TT_INT:
				printf("+ int token: %d\n", t->inum);
				break;
			case TT_FLOAT:
				printf("+ float token: %f\n", t->fnum);
				break;
			case TT_BOP:
				printf("+ operation token: %c\n", t->ch);
				break;
			case TT_LIT:
				printf("+ literal token: %s\n", t->lit);
				break;
			case TT_OPAR:
				printf("+ open parenthese\n");
				break;
			case TT_CPAR:
				printf("+ close parenthese\n");
				break;
			default:
				break;
		}
	}

}

int main(int argc, char *argv[]) {
	/* create expression from args */
	char buffer[512];
	int bufpos = 0;

	for (int i = 1; i<argc; i++) {
		int len = strlen(argv[i]);
		strncpy(&buffer[bufpos], argv[i], len);
		bufpos += len + 1;
		buffer[bufpos-1] = ' ';
	}
	buffer[bufpos] = '\0';
	
	printf("Expression: %s\n", buffer);

	list *tokens = parse_expr(buffer);
	printf("Tokens from infix notation:\n");
	printtokens(tokens);

	list *p = postfix(tokens);
	printf("Tokens in postfix notation:\n");
	printtokens(p);
	
	return 0;
}
