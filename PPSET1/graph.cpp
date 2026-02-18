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
        // this optimized random number gen algorithm was taken from ChatGPT (explained further in report)
        uint64_t key = ((uint64_t)u << 32) ^ (uint64_t)v ^ run_seed;
        uint64_t h = splitmix64(key);
        return (h >> 11) * (1.0 / 9007199254740992.0); 
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
            //debug_printf("Weight (%i,%i): %.2f\n",v1,v2,weight(v1,v2));
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

    std::vector<char> inMST(n, 0);
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

    //precompute log of n
    int ln = log2l(n);

    int C = 1;
    double threshold = 1.0;

    // define cutoff thresholds for optimization
    if (type == 0) {
        //uniform min weight is 1/n on average:
        threshold = 1.0/n;
    } else if (type > 1) {
        //side length for average volume cube of each vertex is (1/n)^(1/type) = n^(-1/type)
        threshold = std::pow(n, -1.0/type);
    }
    bool found = true;

    int u;
    while (pq.getSize() > 0) {

        // only choose new vertex if last iteration found any vertices 
        // (or if we're using type = 1, where there are no thresholds to be concerned about)
        // if not, simply rerun iteration with larger threshold
        if (found || type == 1) {
            u = pq.extractMin();
            inMST[u] = true;
            total += priorities[u];
            debug_printf("new total: %f\n",total);
            found = false;
        }

        if (type != 1) {
            // all complete graph types
            for (int v = 0; v < n; v++) {
                if (!inMST[v]) {
                    double w = weight(u,v);
                    //prune edges above threshold
                    if (w <= C * threshold) {
                        found = true;
                        if (priorities[v] > w) {
                            
                            priorities[v] = w;
                            pq.decreasePriority(v,w);
                        }
                    }
                }
            }

            if (!found) { // threshold is too low, increase it
                C += 1;
                debug_printf("update C --> %i, threshold=%f\n",C,C*threshold);
            }
        } else if (type == 1) { //hypercube graph
            //only non-complete graph type
            //iterate every power of 2
            int power = 1;
            for (int i = 0; i < ln; i++) {
                // only possible values of v1 and v2 such that the difference = 2^i
                // this avoids searching extraneous values for v
                int v1 = u + power;
                int v2 = u - power;
                debug_printf("u = %i, v1 = %i, v2 = %i\n",u, v1 ,v2);
                
                if (0 <= v1 && v1 < n) { // if v1 is a valid vertex
                    if (!inMST[v1]) {
                        double w = weight(u,v1);
                        if (priorities[v1] > w) {   
                            priorities[v1] = w;
                            pq.decreasePriority(v1,w);
                        }
                    }
                }

                if (0 <= v2 && v2 < n) { // if v2 is a valid vertex
                    if (!inMST[v2]) {
                        double w = weight(u,v2);
                        if (priorities[v2] > w) {   
                            priorities[v2] = w;
                            pq.decreasePriority(v2,w);
                        }
                    }
                }
                power *= 2;
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
        std::swap(heap[p],heap[n]);

        //update queue
        std::swap(queue[p],queue[n]);

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