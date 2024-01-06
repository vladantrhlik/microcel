#include "adt/list.h"

#ifndef GRAPH
#define GRAPH

typedef struct {
	list *nodes; /* list if node data */
	list *edges; /* edges stored as indexes to list of nodes */
} graph;

graph *graph_init();

/**
 * Add new node with data to graph
 *
 * @return index of new node in graph 
*/
int graph_add_node(graph *g, void *data);

/**
* Add new oriented edge from one node to another
*
* @param *graph g graph
* @param int from index of the node in which edge starts
* @param int to index of the node in which edge ends
*/
int graph_add_edge(graph *g, int from, int to);

void *graph_get_node(graph *g, int index);



#endif
