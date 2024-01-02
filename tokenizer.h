#ifndef TOKENIZER
#define TOKENIZER

typedef enum {
	TT_EMPTY,	/* white space / no value */
	TT_INT,		/* int value */
	TT_FLOAT,	/* float value */
	TT_LIT,		/* literal value */
	TT_BOP, 	/* binary operation */
	TT_OPAR,	/* ( */
	TT_CPAR,	/* ) */
} token_type; 

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

/**
* Creates array of tokens from given expression
* @param *char expr expression
*/
token **parse_expr(char *expr);

#endif
