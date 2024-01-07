#include "../adt/list.h"

#ifndef TOKENIZER
#define TOKENIZER

/**
 * All supported function
 */
#define FUNC_NAMES {"sin", "cos", "sqrt", "ceil", "floor", "log"}
#define FUNC_FUNCTIONS {sin, cos, sqrt, ceil, floor, log}
#define FUNC_COUNT 6

/**
 * All types of tokens 
 */
typedef enum {
	TT_EMPTY,	/* white space / no value */
	TT_INT,		/* int value */
	TT_FLOAT,	/* float value */
	TT_LIT,		/* literal value */
	TT_BOP, 	/* binary operation */
	TT_OPAR,	/* ( */
	TT_CPAR,	/* ) */
	TT_FUNC,	/* function - sin, cos */
} token_type; 

/**
 * Token structure
 */
typedef struct {
	token_type type;
	union value {
		int inum;
		float fnum;
		char *lit;	
		char ch;
	};
} token;

/**
* Creates new token with given type
* @param token_type type type of new token
*/
token *token_init(token_type type);

void token_free(void **t);

/**
 * Checks if token type is literal, int or float
 */
int isvalue(token_type type);

/**
* @param *char expr expression
* @return stack* stack of all tokens (in reversed order :))
*/
list *parse_expr(char *expr);
#endif
