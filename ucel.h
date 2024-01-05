#include "adt/list.h"

#ifndef UCEL
#define UCEL

typedef struct {
	int width, height;
	list *rows;
} table;

typedef struct {
	char pos[4]; /* A00 - Z99 */
	int evaluated;
	list *tokens;
	float value;
} cell;

table *table_init();

cell *table_add_cell(table *t, int x, int y);

cell *table_get_cell(table *t, int x, int y);

#endif
