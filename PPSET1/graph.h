#ifndef GRAPH_H
#define GRAPH_H

#include "libs.h"

class RandomGraph {
    public:
        RandomGraph(int n, int type);
        ~RandomGraph();
    private:
        int n;
        int type;
        int* edges;
        int* weights;
};

#endif