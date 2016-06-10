#include <stdio.h>
#include "pso.h"

#define MAX_ITER 100

int main() {
    struct swarm *s;
    s = initializeSwarm(50, 0.7, 2.0, 2.0);
    int i;
    for (i = 0; i < MAX_ITER; i++) {
        updateSwarm(s);
    }

    printf("Mejor Resultado: k: %.2f , c: %.2f \n", s->global_best[0], s->global_best[1]);

    return 0;
}
