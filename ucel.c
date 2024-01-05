#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ucel.h"
#include "adt/list.h"

#define DELIM	'|'
#define COL_RANGE 26 /* A - Z */
#define ROW_RANGE 99 /* 0 - 99 */

table *table_init() {
	table *t = malloc(sizeof(table));
	if (!t) return NULL;

	t->width = 0;
	t->height = 0;
	t->rows = list_init();

	return t;
}

cell *table_add_cell(table *t, int x, int y) {
	/* sanity check */
	if (x < 0 || x > COL_RANGE || y < 0 || y > ROW_RANGE) {
		printf("Cell [%d, %d] is out of valid range\n", x, y);
		return NULL;
	}

	/* check if there's enough rows */
	if (y >= t->height - 1) {
		for (int i = 0; i < y - t->height + 1; i++) {
			/* add empty row */
			list_add(t->rows, NULL);
			t->height++;
		}
	}

	list *row = list_get(t->rows, y);
	/* allocate new row if NULL */
	if (!row) {
		list_remove(t->rows, y);
		row = list_init();
		if (!row) return NULL;
		list_insert(t->rows, row, y);
	}

	/* check if there's enough columns in */
	if (x >= row->count - 1) {
		for (int i = 0; i < x - row->count + 1; i++) {
			list_add(row, NULL);
		}
		t->width = x > t->width ? x : t->width;
	}

	/* add new cell */
	cell *c = malloc(sizeof(cell));
	c->evaluated = 0;
	c->value = -1;
	c->tokens = NULL;
	sprintf(c->pos, "%c%d", 'A'+x, y);
	
	/* add cell to row */
	list_remove(row, x);
	list_insert(row, c, x);

	printf("Added cell %s\n", c->pos);

	return c;
}

cell *table_get_cell(table *t, int x, int y) {
	if (t && list_get(t->rows, y)) {
		return list_get(list_get(t->rows, y), x);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	char *file_name = "test_data.ucel";

	FILE *input = fopen(file_name, "r");
	if (!input) {
		printf("Failed to open %s\n", file_name);
		return -1;
	}
	
	char cell_buffer[128];
	char cur, *buf; /* current char in file and in cell buffer */
	int x = 0, y = 0;

	buf = cell_buffer;

	while ( (cur = fgetc(input)) != EOF ) {
		if (cur == DELIM || cur == '\n') {
			/* end of the current cell */
			*buf = '\0';
			printf("[%d,%d]: %s\n", x, y, cell_buffer);
			buf = cell_buffer;

			if (cur == DELIM) x++;
			else y++;
		} else if (buf > cell_buffer || !isspace(cur)) {
			*buf = cur;
			buf++;
		}
	}

	fclose(input);
	
	return 0;
}
