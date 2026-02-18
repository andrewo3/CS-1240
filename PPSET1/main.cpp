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

    double avg = 0.0;
    RandomGraph trial_graph(numpoints, dimension);
    for (int t = 0; t < numtrials; t++) {
        debug_printf("Trial %i\n",t);
        avg += trial_graph.prim();
        trial_graph.reseed();
    }
    avg /= numtrials;
    printf("%f\n",avg);
}