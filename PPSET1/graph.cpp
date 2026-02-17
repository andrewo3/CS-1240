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

    reseed();

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

    std::vector<bool> inMST(n, false);
    std::vector<int> vertices(n);
    std::vector<double> priorities(n);

    const double INF = std::numeric_limits<double>::max();

    for (int i = 0; i < n; i++) {
        vertices[i] = i;
        priorities[i] = INF;
    }
    priorities[0] = 0;
    PriorityQueue pq(n, vertices.data(), priorities.data());
    double total = 0.0;

    while (pq.getSize() > 0) {
        int u = pq.extractMin();

        inMST[u] = true;
        total += priorities[u];
        
        for (int v = 0; v < n; v++) {
            //only non-complete graph type
            if (type == 1 && !ispower2(std::abs(v-u))) continue;

            if (!inMST[v]) {
                double w = weight(u,v);
                if (priorities[v] > w) {
                    priorities[v] = w;
                    pq.decreasePriority(v,w);

                }
            }
        }
    }

    return total;
}

RandomGraph::~RandomGraph() {
    if (type == 2 || type == 3 || type == 4) { // if coords were necessary
        delete[] coords;
    }
}


//build queue
PriorityQueue::PriorityQueue(int n, int* arr, double* priorities) {
    size = n;
    heap.assign(priorities,priorities + n);
    queue.assign(arr,arr + n);
    pos.resize(n);

    for (int i = 0; i < n; i++) {
        pos[queue[i]] = i;
    }

    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapify(i);
    }
}

PriorityQueue::~PriorityQueue() {
}

void PriorityQueue::heapify(int node) {
    int l = left(node);
    int r = right(node);

    int smallest;
    if (l < size && heap[l] < heap[node]) {
        smallest = l;
    } else {
        smallest = node;
    }
    if (r < size && heap[r] < heap[smallest]) {
        smallest = r;
    }
    if (smallest != node) {
        std::swap(heap[node],heap[smallest]);
        std::swap(queue[node],queue[smallest]);

        //update positions
        pos[queue[node]] = node;
        pos[queue[smallest]] = smallest;

        heapify(smallest);
    }
    return;

}

int PriorityQueue::left(int node) {
    return node * 2 + 1;
}

int PriorityQueue::right(int node) {
    return node * 2 + 2;
}

int PriorityQueue::parent(int node) {
    return (node - 1) / 2;
}

int PriorityQueue::extractMin() {
    int min = queue[0];
    heap[0] = heap[size - 1];
    queue[0] = queue[size - 1];
    pos[queue[0]] = 0;
    size--;
    heapify(0);
    pos[min] = -1;
    return min;
}

void PriorityQueue::insert(double v) {
    heap[size] = v;
    int n = size;
    size++;
    while (n != 0 && heap[parent(n)] > heap[n]) {
        //update heap
        int p = parent(n);
        int tmp = heap[p];
        heap[p] = heap[n];
        heap[n] = tmp;

        //update queue
        tmp = queue[p];
        queue[p] = queue[n];
        queue[n] = tmp;

        n = p;
    }
}

void PriorityQueue::decreasePriority(int v, double new_priority) {
    int node = pos[v];
    heap[node] = new_priority;
    
    while (node != 0 && heap[parent(node)] > heap[node]) {
        int p = parent(node);
        
        std::swap(heap[node],heap[p]);
        std::swap(queue[node],queue[p]);

        pos[queue[node]] = node;
        pos[queue[p]] = p;

        node = p;
    }
}