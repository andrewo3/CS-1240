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

    debug_printf("%i total trials.\n", numtrials);

    for (int t = 0; t < numtrials; t++) {
        printf("Trial %i\n",t);
        RandomGraph trial_graph(numpoints, dimension);
        for ( int i = 0; i < 5; i++) {
            trial_graph.prim();
        }
    }
}