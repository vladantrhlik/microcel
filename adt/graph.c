#include "graph.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

graph *graph_init() {
	graph *g = malloc(sizeof(graph));
	if (!g) return NULL;

	g->edges = list_init();
	g->nodes = list_init();

	return g;
}

int graph_add_node(graph *g, void *data) {
	/* sanity check */
	if (!g || !data) return -1;
	
	/* add to nodes */
	list_add(g->nodes, data);
	/* add new list for its neighbours */
	list_add(g->edges, list_init());

	return g->nodes->count - 1;
}

int graph_index_valid(graph *g, int index) {
	if (!g) return 0;
	if (index >= 0 && index < g->nodes->count) return 1;
	return 0;
}

int graph_add_edge(graph *g, int from, int to) {
	if (!g) return -1;

	if (!graph_index_valid(g, from) || !graph_index_valid(g, to)) {
		printf("Invalid node index\n");
		return -1;
	}
	
	list_add(list_get(g->edges, from), (void *) to);
	return 0;
}

void *graph_get_node(graph *g, int index) {
	if (!g || graph_index_valid(g, index)) return NULL;

	
}


