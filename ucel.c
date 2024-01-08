#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ucel.h"
#include "adt/list.h"
#include "eval/eval.h"
#include "eval/tokenizer.h"
#include "eval/substitute.h"
#include "eval/analyzer.h"
#include "utils.h"

#define DELIM	'|'
#define COL_RANGE 26 /* A - Z */
#define ROW_RANGE 99 /* 0 - 99 */
	
cell EMPTY_CELL = {"", 1, C_EMPTY};

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
		t->width = x+1 > t->width ? x+1 : t->width;
	}

	/* add new cell */
	cell *c = malloc(sizeof(cell));
	if (!c) return NULL;

	c->dependencies = list_init();
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

int chars_to_pos(char *chars, int *x, int *y) {
	if (chars[0] >= 'A' && chars[0] <= 'Z') *x = chars[0]-'A';
	else return 0;
	
	chars++;
	
	*y = 0;
	while (*chars) {
		if (isdigit(*chars)) {
			if (*y > 0) *y*=10;
			*y += *chars-'0';
		} else return 0;
		chars++;
	}

	return 1;
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

int load_cell_dependencies(table *t) {
	token *tok;
	cell *c, *d;
	int xp, yp;

	for (int y = 0; y < t->height; y++) {
		for (int x = 0; x < t->width; x++) {
			c = table_get_cell(t, x, y);
			if (!c) continue;
			if (c->type == C_EXPR) {
				/* substitute constants */
				substitute(c->tokens);
				for (int i = 0; i < c->tokens->count; i++) {
					tok = list_get(c->tokens, i);
					if (tok->type == TT_LIT) {
						/* parse cell position */
						if (chars_to_pos(tok->lit, &xp, &yp)
							&& xp >= 0 && xp < t->width
							&& yp >= 0 && yp < t->height) {
							d = table_get_cell(t, xp, yp);
							if (d) list_add(c->dependencies, d);
						} else {
							printf("'%s' is not a valid cell\n", tok->lit);
							return 0;
						}
					}
				}
			}
		}
	}
	
	return 1;
}

int eval_cell(table *t, cell *c) {
	if (!c || c->evaluated) return -1;

	if (c->type == C_EXPR && !c->evaluated) {
		/* evaluate all dependencies */
		for (int i = 0; i<c->dependencies->count; i++) {
			cell *d = list_get(c->dependencies, i);
			eval_cell(t, d);
		}
		/* replace cells with their value */
		for (int i = 0; i<c->tokens->count; i++) {
			token *tok = list_get(c->tokens, i);
			if (tok->type == TT_LIT){
				int x, y;
				if (chars_to_pos(tok->lit, &x, &y)) {
					cell *c = table_get_cell(t, x, y);
					if (c->type == C_NUM) {
						free(tok->lit);
						tok->fnum = c->value;
					}
					else {
						printf("Referencing cell '%s' with no value, using 0\n", tok->lit);
						tok->fnum = 0;
					}
					tok->type = TT_FLOAT;
				}
			}
		}
		analyze(c->tokens);
		float res = 0;
		eval(c->tokens, &res);
		/* replace token cell with value cell */
		/* free list of tokens */
		list_free(&c->tokens, &token_free);
		/* change cell to float value */
		c->type = C_NUM;
		c->value = res;
	} 
	
	c->evaluated = 1;

	return 0;	
}

int eval_table(table *t) {
	/* sanity check */
	if (!t) return -1;
	/* load dependencies of all cells */
	if (!load_cell_dependencies(t)) return -1;

	for (int y = 0; y < t->height; y++) {
		for (int x = 0; x < t->width; x++) {
			cell *c = table_get_cell(t, x, y);
			if (c && !c->evaluated) eval_cell(t, c);
		}
	}
	
	return 0;
}

void print_table(table *t) {
	if (!t) return;

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
}

void cell_free(void **cellp) {
	if (!cellp || !*cellp || ((cell*) *cellp) == &EMPTY_CELL) return;

	cell *cell = *cellp;
	printf("Freeing cell\n");

	/* free dependencies */
	list_free(&cell->dependencies, NULL);

	/* free text if any exists */
	if (cell->type == C_STR && cell->txt) {
		free(cell->txt);
	}

	free(cell);
	*cellp = NULL;
}

void table_free(table **tp) {
	/* sanity check */
	if (!tp || !*tp) return;

	table *t = *tp;

	/* free all rows */
	for (int y = 0; y < t->height; y++) {
		printf("Freeing row %d\n", y);
		list *row = list_get(t->rows, y);
		list_free(&row, cell_free);
		printf("Row %d freed\n", y);
	}
	/* free list of rows */
	list_free(&t->rows, NULL);
	
	free(t);
	*tp = NULL;
}
