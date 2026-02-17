#include "libs.h"

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Usage: %s <debug> <numpoints> <numtrials> <dimension>\n", argv[0]);
        return 1;
    }
    
    DEBUG = atoi(argv[1]);
    int numpoints = atoi(argv[2]);
    int numtrials = atoi(argv[3]);
    int dimension = atoi(argv[4]);

    if (dimension > 4 || dimension < 0) {
        throw std::invalid_argument("dimension must be between 0 and 4, inclusive.");
    }

    debug_printf("%i total trials.\n", numtrials);

    std::vector<double> msts;
    for (int t = 0; t < numtrials; t++) {
        debug_printf("Trial %i\n",t);
        RandomGraph trial_graph(numpoints, dimension);
        for ( int i = 0; i < 5; i++) {
            msts.push_back(trial_graph.prim());
        }
    }

    double avg = 0.0;
    for (int i = 0; i < msts.size(); i++) {
        avg += msts[i];
    }
    avg /= msts.size();
    printf("%f\n",avg);
}