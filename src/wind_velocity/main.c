#include <stdio.h>
#include "pso.h"
#include <time.h>

char *file_path[] = {
    "../../data/2015/parse/freq_data_15.csv",
    "../../data/2014/parse/freq_data_14.csv",
    "../../data/2013/parse/freq_data_13.csv",
    "../../data/All/parse/freq_data_All.csv"
};

int main() {
    int file, i;
    float k, c;
    struct swarm *s;
    clock_t start;
    clock_t end;
    float seconds;

    for (file = 0; file < 4; file++) {
        start = clock();

        printf("\n File %s \n", file_path[file]);    
        s = initializeSwarm(50, PARAM_W, PARAM_C1, PARAM_C2, file_path[file]);

        for (i = 0; i < MAX_ITER; i++) {
           updateSwarm(s);
	       updateParameters(s,i);
        }

        k = s->global_best[0];
        c = s->global_best[1];
        printf("Mejor Resultado: k: %.2f , c: %.2f \n", k, c);
        end = clock();
        seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Time in sec of run: %f", seconds);
    }

    return 0;
}