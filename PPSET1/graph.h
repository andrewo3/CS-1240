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
class PriorityQueue {
    public:
        PriorityQueue(int n, int* arr, double* priorities);
        ~PriorityQueue();
        void heapify(int node);
        int extractMin();
        void insert(double v);
        int getSize() {
            return size;
        }
        void decreasePriority(int node, double new_priority);

    private:
        int size;
        std::vector<double> heap;
        std::vector<int> queue;
        std::vector<int> pos;
        int left(int node);
        int right(int node);
        int parent(int node);
        

};



#endif