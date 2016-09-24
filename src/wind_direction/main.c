#include <stdio.h>
#include "pso.h"
#include "freq/freq.h"

int main() {
    int i, j;
    float k, c;
    struct swarm *s;
    s = initializeSwarm(500, PARAM_W, PARAM_C1, PARAM_C2);


    printf("Initial \n");
    for ( j = 0; j < LEN_SOL; j++) {
        printf(" %.5f ", s->global_best[j]);        
    }

    printf("\n");

    printf("Best: %lf \n", objectiveFunction(s->global_best));

    for (i = 0; i < MAX_ITER; i++) {
        updateSwarm(s);
	    updateParameters(s,i);
    }

    printf("Best #: %d \n", i);
    for ( j = 0; j < LEN_SOL; j++) {
        printf(" %.5f,", s->global_best[j]);        
    }

    printf("\n");

    printf("Best: %lf \n", objectiveFunction(s->global_best));

    /*printf("\nBest result: \n");
    for ( i = 0; i < LEN_SOL; i++) {
      printf("Value: %.5f \n", s->global_best[i]);        
    }
    printf("Best: %lf \n", objectiveFunction(s->global_best));*/
    return 0;
}