#include "adt/list.h"

#ifndef EVAL
#define EVAL

float eval(list *tokens);

/* 
 * Rearanges tokens in infix notation to postfix (Polish) notation 
*/
list *postfix(list *tokens);
#endif
