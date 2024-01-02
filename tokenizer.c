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

/**
 * @param token_type cur current type of token in buffer
 * @param char c new character from expression
 * @return token_type type of token depending on input char
 */
token_type ttfromc(token_type *cur, char c) {
	if (isdigit(c)) {
		if (*cur == TT_FLOAT) return TT_FLOAT;
		else if(*cur == TT_LIT) return TT_LIT;
		else return TT_INT;
	} else if (c == '+' || c == '-' || c == '*' || c == '/') {
		return TT_BOP;
	} else if (c == '.') {
		if (*cur == TT_INT) {
			*cur = TT_FLOAT;
			return TT_FLOAT;
		} else if (*cur == TT_FLOAT) {
			/* probably two dots in one number */
			printf("Two '.' symbols in one float\n");
			return TT_EMPTY;
		}
		return TT_FLOAT;
	} else if (isspace(c)) {
		return TT_EMPTY;
	} else if (isalpha(c)) {
		return TT_LIT;
	} 
	else if (c == '(') return TT_OPAR;
	else if (c == ')') return TT_CPAR;

	printf("Unexpected token '%c'\n", c);
	return TT_EMPTY;
}

token **parse_expr(char *expr) {
	if (!expr) return NULL;

	char buffer[32];
	memset(buffer, '\0', 32); /* clear buffer */
	token_type curtype = TT_EMPTY;
	int bufpos = 0;

	while (*expr) {
		token_type newtype = ttfromc(&curtype, *expr);
		
		if (newtype != curtype) {
			/* generate new token */	
			buffer[bufpos] = '\0';
			token *t = token_init(curtype);
			/* save data from buffer */
			switch (curtype) {
				case TT_INT:
					t->inum = atoi(buffer);
					printf("+ int token: %d\n", t->inum);
					break;
				case TT_FLOAT:
					t->fnum = atof(buffer);
					printf("+ float token: %f\n", t->fnum);
					break;
				case TT_BOP:
					t->ch = *buffer;
					printf("+ operation token: %c\n", t->ch);
					break;
				case TT_LIT:
					t->lit = malloc(sizeof(char) * strlen(buffer));
					if (!t->lit) return NULL;
					strcpy(t->lit, buffer);
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
			/* reset buffer */
			bufpos = 0;
			memset(buffer, '\0', 32); /* clear buffer */
			curtype = newtype;
		} 		
		
		/* add new character to buffer */
		if (newtype == curtype || curtype == TT_EMPTY) {/* add char to buffer */
			buffer[bufpos] = *expr;
			bufpos++;
		} 

		expr++;
	}
	return NULL;
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

	parse_expr(buffer);
	return 0;
}
