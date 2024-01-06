#include <stdio.h>
#include <string.h>
#include "adt/list.h"
#include "eval/tokenizer.h"
#include "eval/eval.h"
#include "eval/analyzer.h"
#include "eval/substitute.h"
#include "ucel.h"

void printtokens(list *tokens) {
	/* print all tokens */
	for (int i = 0; i<tokens->count; i++) {
		token *t = list_get(tokens, i);

		if (!t) {
			printf("! NULL token\n");
			continue;
		}
		
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
			case TT_FUNC:
				printf("+ function: %s\n", t->lit);
				break;	
			default:
				break;
		}
	}

}

void eval_expr_from_args(int argc, char *argv[]) {
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
	
	/* split expression into list of tokens */
	list *tokens = parse_expr(buffer);
	printf("Tokens from infix notation:\n");
	printtokens(tokens);

	/* substitute literals by constants */
	substitute(tokens);

	/* TODO: cell dependency checking */
	
	/* analyze expression structure, fix '-' */
	analyze(tokens);
	printf("Tokens after analysis:\n");
	printtokens(tokens);

	/* evaluate expression */
	float result;
	eval(tokens, &result);

	printf("Result: %f\n", result);


}

int main(int argc, char *argv[]) {

	char *file_name = "test_data.ucel";
	table *t = parse_table(file_name);

	if (!t) printf("Table parsing failed\n");

	eval_table(t);	
	printf("Eval done.\n");

	/* print table */
	for (int y = 0; y < t->height; y++) {
		for (int x = 0; x < t->width; x++) {
			cell *c = table_get_cell(t, x, y);
			if (!c) continue;
			switch (c->type) {
				case C_EMPTY:
					break;
				case C_NUM:
					printf("%f", c->value);
					break;
				case C_STR:
					printf("%s", c->txt);
					break;
				case C_EXPR:
					printf("expr.");
					break;
			}
			printf("\t|");
		}
		printf("\n");
	}

	return 0;
}
