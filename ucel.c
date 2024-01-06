#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ucel.h"
#include "adt/list.h"
#include "eval/tokenizer.h"

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
	
	static cell EMPTY_CELL = {0};
	EMPTY_CELL.type = C_EMPTY;

	/* check if there's enough rows */
	if (y >= t->height - 1) {
		for (int i = 0; i < y - t->height + 1; i++) {
			/* add empty row */
			list_add(t->rows, list_init());
			t->height++;
		}
	}

	list *row = list_get(t->rows, y);

	/* check if there's enough columns in */
	if (x >= row->count - 1) {
		for (int i = 0; i < x - row->count + 1; i++) {
			list_add(row, (void*) &EMPTY_CELL);
		}
		t->width = x > t->width ? x : t->width;
	}

	/* add new cell */
	cell *c = malloc(sizeof(cell));
	if (!c) return NULL;

	c->evaluated = 0;
	c->value = -1;
	c->tokens = NULL;
	sprintf(c->pos, "%c%d", 'A'+x, y);
	
	/* add cell to row */
	list_remove(row, x);
	list_insert(row, c, x);

	return c;
}

cell *table_get_cell(table *t, int x, int y) {
	if (t && list_get(t->rows, y)) {
		return list_get(list_get(t->rows, y), x);
	}
	return NULL;
}

table *parse_table(char *file_name) {
	FILE *input = fopen(file_name, "r");
	if (!input) {
		printf("Failed to open %s\n", file_name);
		return NULL;
	}
	
	char cell_buffer[128];
	char cur, *buf; /* current char in file and in cell buffer */
	int x = 0, y = 0;
	table *t = table_init();

	if (!t) return NULL;

	buf = cell_buffer;

	while ( (cur = fgetc(input)) != EOF ) {
		if (cur == DELIM || cur == '\n') {
			if (buf > cell_buffer) {
				/* end of the current cell */
				cell *c = table_add_cell(t, x, y);
				if (!c) return NULL;

				*buf = '\0';
				if (*cell_buffer == '=') {
					sprintf(c->pos, "%c%d", 'A'+x, y);
					c->type = C_EXPR;
					c->evaluated = 0;
					c->tokens = parse_expr(cell_buffer+1);
				} else {
					/* find out type of value (int/float/char*) */
					buf = cell_buffer;
					c->evaluated = 1;
					int nan = 0;
					while (*buf) {
						if ( !(isspace(*buf) || *buf == '.' || isdigit(*buf)) ) nan = 1;
						buf++;
					}
					if (nan) {
						c->type = C_STR;
						c->txt = malloc(sizeof(char) + (strlen(cell_buffer + 1)));
						if (!c->txt) return NULL;
						strcpy(c->txt, cell_buffer);
					} else {
						c->type = C_NUM;
						c->value = atof(cell_buffer);
					}
				}
				buf = cell_buffer;
			}
			if (cur == DELIM) x++;
			else (x = 0, y++);
		} else if (buf > cell_buffer || !isspace(cur)) {
			*buf = cur;
			buf++;
		}
	}

	fclose(input);
	
	return t;
}
