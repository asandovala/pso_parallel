#include <stdio.h>
#include "pso.h"

#define MAX_ITER 500

int main() {
    int i;
    float k, c;
    struct swarm *s;
    s = initializeSwarm(100, 0.7, 1.5, 2.0);

    for (i = 0; i < MAX_ITER; i++) {
        updateSwarm(s);
        //printf("Resultado: k: %.2f , c: %.2f \n", s->global_best[0], s->global_best[1]);
    }

    k = s->global_best[0];
    c = s->global_best[1];
    printf("Mejor Resultado: k: %.2f , c: %.2f \n", k, c);

    return 0;
}
