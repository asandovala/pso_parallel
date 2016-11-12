#include <stdio.h>
#include "pso.h"
#include "freq/freq.h"

int main() {
    int i, j;
    float k, c;
    struct swarm *s;
    double pi;
    double sum = 0.0;
    s = initializeSwarm(100);


    printf("Initial \n");
    for ( j = 0; j < LEN_SOL; j++) {
        printf(" %.5f ", s->global_best[j]);        
    }

    printf("\n");
    printf("Fitness: %lf \n", objectiveFunction(s->global_best));

    for (i = 0; i < MAX_ITER; i++) {
        //printf("\n Iteration %d \n", i);
        updateSwarm(s);
        updateParameters(s, i);

        if (objectiveFunction(s->global_best) < 22.362) {
            break;
        }
    }

    printf("\nIterations #: %d \n", i);
    for ( j = 0; j < LEN_SOL; j++) {
        printf(" %.5f,", s->global_best[j]);        
    }
    printf("\n");
    printf("Fitness: %lf \n", objectiveFunction(s->global_best));
  
    return 0;
}