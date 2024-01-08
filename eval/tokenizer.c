#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "../adt/list.h"

#define LOG 0

token *token_init(token_type type) {
	if (!type) return NULL;

	token *t = malloc(sizeof(token));
	if (!t) return NULL;

	t->type = type;
	return t;
}

void token_free(void **tp) {
	if (!tp || !*tp) return;
	token *t = *tp;

	if ( (t->type == TT_LIT || t->type == TT_FUNC) && t->lit) {
		free(t->lit);
	}
	free(*tp);
	*tp = NULL;
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
	} else if (isspace(c) || !c) {
		return TT_EMPTY;
	} else if (isalpha(c)) {
		return TT_LIT;
	} 
	else if (c == '(') return TT_OPAR;
	else if (c == ')') return TT_CPAR;

	printf("Unexpected token '%c'\n", c);
	return TT_EMPTY;
}

/**
 * @return int 1 if token holds value else 0
 */
int isvalue(token_type type) {
	return type == TT_LIT || type == TT_INT || type == TT_FLOAT;
}

/**
 * @return int 1 if token is operator or parenthese
 */
int issinglechar(token_type t) {
	return t == TT_BOP || t == TT_CPAR || t == TT_OPAR;
}

/**
 * @return int 1 if string is name of predefined function else 0
 */
int isfunc(char *lit) {
	char *funcs[] = FUNC_NAMES;

	for (int i = 0; i<FUNC_COUNT; i++) {
		if (!strcmp(lit, funcs[i])) return 1;
	}
	return 0;
}

list *parse_expr(char *expr) {
	if (!expr) return NULL;

	char buffer[32];
	memset(buffer, '\0', 32); /* clear buffer */
	token_type curtype = TT_EMPTY;
	int bufpos = 0;

	list *l = list_init();

	if (LOG) printf("parsing expression %s\n", expr);

	while (*expr || bufpos > 0) {
		token_type newtype = ttfromc(&curtype, *expr);
			
	
		if ( newtype != curtype || (newtype == curtype && issinglechar(newtype)) || !*expr ) {
			/* generate new token */	
			buffer[bufpos] = '\0';
			token *t = token_init(curtype);
			/* save data from buffer */
			switch (curtype) {
				case TT_INT:
					t->inum = atoi(buffer);
					if (LOG) printf("int value: %d\n", t->inum);
					break;
				case TT_FLOAT:
					t->fnum = atof(buffer);
					if (LOG) printf("float value: %f\n", t->fnum);
					break;
				case TT_BOP:
					t->ch = *buffer;
					if (LOG) printf("binary op: %c\n", t->ch);
					break;
				case TT_LIT:
					t->lit = malloc(sizeof(char) * strlen(buffer));
					if (!t->lit) return NULL;
					strcpy(t->lit, buffer);

					if (isfunc(t->lit)) t->type = TT_FUNC;
					if (LOG) printf("literat/func: %s\n", t->lit);
					break;
				case TT_OPAR:
					if (LOG) printf("open parenthese");
					break;
				case TT_CPAR:
					if (LOG) printf("close parenthese");
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
		} else if (curtype == TT_BOP && *expr == '-') {
			printf("fixing '-'\n");
			/* fix '-' after other BOP */
			newtype = TT_INT;
			curtype = TT_INT;
		}

		if (!*expr) break;
	
		/* add new character to buffer */
		if (newtype == curtype || curtype == TT_EMPTY) {/* add char to buffer */
			buffer[bufpos] = *expr;
			bufpos++;
		} 

		expr++;
	}
	return l;
}
