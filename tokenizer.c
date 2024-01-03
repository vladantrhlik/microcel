#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "adt/list.h"

token *token_init(token_type type) {
	if (!type) return NULL;

	token *t = malloc(sizeof(token));
	if (!t) return NULL;

	t->type = type;
	return t;;
}

/**
 * token_type from char -- Figures out type of currently read token
 *
 * @param token_type cur current type of token in buffer
 * @param char c new character from expression
 * @return token_type type of token depending on input char
 */
token_type ttfromc(token_type *cur, char c) {
	if (isdigit(c)) {
		if (*cur == TT_FLOAT) return TT_FLOAT;
		else if(*cur == TT_LIT) return TT_LIT;
		else return TT_INT;
	} else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
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

int issinglechar(token_type t) {
	return t == TT_BOP || t == TT_CPAR || t == TT_OPAR;
}

list *parse_expr(char *expr) {
	if (!expr) return NULL;

	char buffer[32];
	memset(buffer, '\0', 32); /* clear buffer */
	token_type curtype = TT_EMPTY;
	int bufpos = 0;

	list *l = list_init();

	while (*expr) {
		token_type newtype = ttfromc(&curtype, *expr);

		if (newtype != curtype || (newtype == curtype && issinglechar(newtype)) ) {
			/* generate new token */	
			buffer[bufpos] = '\0';
			token *t = token_init(curtype);
			/* save data from buffer */
			switch (curtype) {
				case TT_INT:
					t->inum = atoi(buffer);
					break;
				case TT_FLOAT:
					t->fnum = atof(buffer);
					break;
				case TT_BOP:
					t->ch = *buffer;
					break;
				case TT_LIT:
					t->lit = malloc(sizeof(char) * strlen(buffer));
					if (!t->lit) return NULL;
					strcpy(t->lit, buffer);
					break;
				case TT_OPAR:
					break;
				case TT_CPAR:
					break;
				default:
					break;
			}
			/* add to stack */
			list_add(l, t);
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
	return l;
}
