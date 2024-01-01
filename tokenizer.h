#ifndef TOKENIZER
#define TOKENIZER

typedef enum {
	TT_EMPTY,
	TT_INT,		/* int value */
	TT_FLOAT,	/* float value */
	TT_LIT,		/* literal value */
	TT_BOP, 	/* binary operation */
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

token *token_init(token_type type);

token **parse_expr(char *expr);

#endif
