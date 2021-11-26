#ifndef READ_GRAPH_H
#define READ_GRAPH_H 0
#include <cstdio>

int read_graph(
	int ** W,
	int ** W_index,
	int * exist,
	int * id,
	FILE * file0,
	int DEG,
	int N);


int read_N(
	FILE * file0
	);


#endif
