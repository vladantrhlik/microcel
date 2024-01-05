#include "adt/list.h"

#ifndef EVAL
#define EVAL

/**
 * Evaluates expression split into VALID TOKENS
 + @return float result of expression
*/
int eval(list *tokens, float *res);

/* 
 * Rearanges tokens in infix notation to postfix (Polish) notation 
*/
list *postfix(list *tokens);

#endif
