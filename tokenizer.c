#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

token *token_init(token_type type) {
	if (!type) return NULL;

	token *t = malloc(sizeof(token));
	if (!t) return NULL;

	t->type = type;
	return t;;
}

token **parse_expr(char *expr) {
	if (!expr) return NULL;

	char buffer[32];
	token_type curtype = TT_EMPTY;
	int bufpos = 0;

	while (*expr) {
		printf("new char: %c\n", *expr);
		token_type newtype = TT_EMPTY;
		/* expected type of token */
		if (isdigit(*expr)) {
			if (curtype == TT_FLOAT) newtype = TT_FLOAT;
			else newtype = TT_INT;
		} else if (*expr == '+' || *expr == '-' || 
				*expr == '*' || *expr == '/') {
			newtype = TT_BOP;
		} else if (*expr == '.') {
			if (curtype == TT_INT) {
				newtype = TT_FLOAT;
				curtype = TT_FLOAT;
			} else if (curtype == TT_FLOAT) {
				/* probably two dots in one number */
				printf("Two '.' symbols in one float\n");
			}
		} else if (isspace(*expr)) {
			/* skip white spaces */
			expr++;
			continue;
		} else {
			printf("Unexpected token '%c'\n", *expr);
			return NULL;
		}

		if (newtype == curtype || curtype == TT_EMPTY) {/* add char to buffer */
			buffer[bufpos] = *expr;
			bufpos++;
		} else if (newtype != curtype) {
			/* output old token */	
			buffer[bufpos] = '\0';
			token *t = token_init(curtype);
			/* save data from buffer */
			switch (curtype) {
				case TT_INT:
					t->inum = atoi(buffer);
					printf("int token: %d\n", t->inum);
					break;
				case TT_FLOAT:
					t->fnum = atof(buffer);
					printf("float token: %f\n", t->fnum);
					break;
				case TT_BOP:
					t->ch = *buffer;
					printf("operation token: %c\n", t->ch);
					break;
				case TT_LIT:
					t->lit = malloc(sizeof(char) * strlen(buffer));
					if (!t->lit) return NULL;
					strcpy(t->lit, buffer);
					printf("literal token: %s\n", t->lit);
					break;
			}

			curtype = newtype;
			bufpos = 0;
		} 		
		printf("buffer: %s\n", buffer);
		expr++;
	}
}

int main(int argc, char *argv[]) {
	char *test = "42 + 3 * 11";

	parse_expr(test);
	return 0;
}
