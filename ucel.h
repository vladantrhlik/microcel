#include "adt/list.h"

#ifndef UCEL
#define UCEL

typedef enum {
	C_EXPR,
	C_NUM,
	C_STR,
	C_EMPTY,
} cell_type;

typedef struct {
	int width, height;
	list *rows;
} table;

typedef struct {
	char pos[4]; /* A00 - Z99 */
	int evaluated;
	cell_type type;
	union {
		list *tokens;
		float value;
		char *txt;
	};
} cell;

table *table_init();

cell *table_add_cell(table *t, int x, int y);

cell *table_get_cell(table *t, int x, int y);

table *parse_table(char *file_name);

#endif
