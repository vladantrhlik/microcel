#include "../adt/list.h"

#ifndef UCEL
#define UCEL

/**
 * Type of cell
 */
typedef enum {
	C_EXPR,
	C_NUM,
	C_STR,
	C_EMPTY,
} cell_type;

/**
 * Current state of cell
 */
typedef enum {
	EVALUATED,
	NEVALUATED,
	EVALUATING, /* for detecting cyclic dependencies */
} eval_state;

/**
 * Table structure
 */
typedef struct {
	int width, height;
	list *rows;
} table;

typedef struct {
	char pos[4]; /* A00 - Z99 */
	eval_state state;
	cell_type type;
	list *dependencies; /* pointers to all cell which need to be evaluated */
	union {
		list *tokens;
		float value;
		char *txt;
	};
} cell;

table *table_init();

void table_free(table **t);

cell *table_add_cell(table *t, int x, int y);

cell *table_get_cell(table *t, int x, int y);

table *parse_table(char *file_name);

void print_table(table *t);

int load_cell_dependencies(table *t);

int eval_table(table *t);

/**
* Converts characters to position (B2 -> [1, 1])
*/
int chars_to_pos(char *chars, int *x, int *y);

#endif
