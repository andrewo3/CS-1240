#include "libs.h"

RandomGraph::RandomGraph(int n, int type) {
    this->n = n;
    this->type = type;
    printf("Constructing graph with %d points and type %d\n", n, type);
    this->edges = (int*)malloc((n * n )* sizeof(int));
}

RandomGraph::~RandomGraph() {
}