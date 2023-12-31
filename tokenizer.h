#ifndef TOKENIZER
#define TOKENIZER

enum token_type {
	TT_INT,
	TT_FLOAT,
	TT_LIT,
	TT_BOP,
};

typedef struct {
	token_type type;
	union value {
		int inum;
		float fnum;
		char *txt;	
	};
} token;



#endif
