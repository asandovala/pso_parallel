#include <stdio.h>
#include "pso.h"
#include "freq/freq.h"

int main() {
    int i, j;
    float k, c;
    struct swarm *s;
    double pi;
    double sum = 0.0;
    s = initializeSwarm(100, PARAM_W, PARAM_C1, PARAM_C2);


    printf("Initial \n");
    for ( j = 0; j < LEN_SOL; j++) {
        printf(" %.5f ", s->global_best[j]);        
    }

    printf("\n");

    printf("Best: %lf \n", objectiveFunction(s->global_best));
    printf("Prob solution: \n");
    for (i = 0; i < NUMBER_OF_CLASSES; i++) {
        pi = probabilityWindDirection(s->global_best, i);
        printf(" %lf ", pi);
        sum += pi;
    }
    printf("\n sum: %lf \n", sum);

    for (i = 0; i < MAX_ITER; i++) {
        updateSwarm(s);
	    updateParameters(s,i);
    }

    printf("\nIterations #: %d \n", i);
    for ( j = 0; j < LEN_SOL; j++) {
        printf(" %.5f,", s->global_best[j]);        
    }

    printf("\n");

    printf("Best: %lf \n", objectiveFunction(s->global_best));
    printf(" Prob solution \n");
    sum = 0.0;
    for (i = 0; i < NUMBER_OF_CLASSES; i++) {
        pi = probabilityWindDirection(s->global_best, i);
        printf(" %lf ", pi);
        sum += pi;
    }
    printf("\n sum: %lf \n", sum);
  
    return 0;
}