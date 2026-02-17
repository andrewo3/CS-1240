#include "libs.h"


bool ispower2(int d) {
    return d > 0 && (d & (d - 1)) == 0;
}

double dist(int n, double* coord1, double* coord2) {
    double d = 0;
    for (int i = 0; i < n; i++) {
        double diff = (coord2[i] - coord1[i]);
        d += diff * diff;
    }
    return std::sqrt(d);
}

RandomGraph::RandomGraph(int n, int type) {
    this->n = n;
    this->type = type;
    
    printf("Reseed start\n");
    reseed();
    printf("Reseed end\n");

    if (type == 2 || type == 3 || type == 4) {
        //generate coordinates if needed
        coords = new double[type * n];
    }
}


// generate weights for edges of complete graphs on demand
// so they dont need to be stored in memory
double RandomGraph::weight(int u, int v) {
    if (type < 2) {
        // unique seed for pair of vertices (unique edge value)
        // also seeded by run seed
        std::seed_seq seq{u, v, run_seed}; 
        std::mt19937 vertex_rng(seq);
        std::uniform_real_distribution<double> uniform(0.0, 1.0);
        return uniform(vertex_rng);
    } else {
        return dist(type,&coords[type * u],&coords[type * v]);
    }
}

void RandomGraph::reseed() {
    //seed global randomness
    std::random_device rd;
    run_seed = rd();           // store a fixed seed for this run
    global_gen.seed(run_seed);
}

double RandomGraph::prim() {
    for (int v1 = 0; v1 < n; v1++) {
        for (int v2 = v1 + 1; v2 < n; v2++) {
            debug_printf("Weight (%i,%i): %.2f\n",v1,v2,weight(v1,v2));
        }
    }
    if (type == 2 || type == 3 || type == 4) {
        // dimensional graph (unit square/cube/hypercube coordinates)
        
        //initialize random engine with random seed
        std::default_random_engine coord_gen(global_gen());
        //create uniform distibution
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        for (int c = 0; c < type * n; c++) {
            coords[c] = distribution(coord_gen);
        }
    }



    return 0.0;
}

RandomGraph::~RandomGraph() {
    if (type == 2 || type == 3 || type == 4) { // if coords were necessary
        delete[] coords;
    }
}


//build queue
MinHeap::MinHeap(int n, int* arr) {
    int* heap = new int[n];
}

MinHeap::~MinHeap() {
    delete[] heap;
}

void MinHeap::minHeapify(int* heap, int node) {

}

int MinHeap::left(int node) {
    return node * 2 + 1;
}

int MinHeap::right(int node) {
    return node * 2 + 2;
}

void insert(int n) {

}