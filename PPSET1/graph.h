#ifndef GRAPH_H
#define GRAPH_H

#include "libs.h"

bool ispower2(int d);

double dist(int n, double* coord1, double* coord2);

class RandomGraph {
    public:
        RandomGraph(int n, int type);
        ~RandomGraph();
        double prim();
        double weight(int u, int v);
        void reseed();
    private:
        int n;
        int type;
        double* coords;
        //initialize random engine for weight generation with random seed
        std::mt19937 global_gen;
        int run_seed = 0;
};


//min heap with relevant minimum extracting algorithm
class MinHeap {
    public:
        MinHeap(int n, int* arr);
        ~MinHeap();
        void minHeapify(int* heap, int node);
        int extractMin();

    private:
        int* heap;
        int left(int node);
        int right(int node);

}

#endif