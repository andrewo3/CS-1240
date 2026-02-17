#include "libs.h"

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Usage: %s <debug> <numpoints> <numtrials> <dimension>\n", argv[0]);
        return 1;
    }
    
    int debug = atoi(argv[1]);
    int numpoints = atoi(argv[2]);
    int numtrials = atoi(argv[3]);
    int dimension = atoi(argv[4]);

    if (debug == 0) {

    } else if (debug == 1) { // test graph construction
        RandomGraph graph(numpoints, dimension);

    }
}