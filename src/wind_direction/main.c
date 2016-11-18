#include <stdio.h>
#include "pso.h"
#include "freq/freq.h"
#include <time.h>

char *file_path[] = {
    "../../data/2015/parse/direction_freq_data_15.csv",
    "../../data/2014/parse/direction_freq_data_14.csv",
    "../../data/2013/parse/direction_freq_data_13.csv",
    "../../data/All/parse/direction_freq_data_ENE.csv",
    "../../data/All/parse/direction_freq_data_FEB.csv",
    "../../data/All/parse/direction_freq_data_MAR.csv",
    "../../data/All/parse/direction_freq_data_ABR.csv",
    "../../data/All/parse/direction_freq_data_MAY.csv",
    "../../data/All/parse/direction_freq_data_JUN.csv",
    "../../data/All/parse/direction_freq_data_JUL.csv",
    "../../data/All/parse/direction_freq_data_AGO.csv",
    "../../data/All/parse/direction_freq_data_SEP.csv",
    "../../data/All/parse/direction_freq_data_OCT.csv",
    "../../data/All/parse/direction_freq_data_NOV.csv",
    "../../data/All/parse/direction_freq_data_DIC.csv",
    "../../data/2015/parse/direction_freq_data_15_ENE.csv",
    "../../data/2014/parse/direction_freq_data_14_ENE.csv",
    "../../data/2013/parse/direction_freq_data_13_ENE.csv",
    "../../data/2015/parse/direction_freq_data_15_MAY.csv",
    "../../data/2014/parse/direction_freq_data_14_MAY.csv",
    "../../data/2013/parse/direction_freq_data_13_MAY.csv",
    "../../data/2015/parse/direction_freq_data_15_SEP.csv",
    "../../data/2014/parse/direction_freq_data_14_SEP.csv",
    "../../data/2013/parse/direction_freq_data_13_SEP.csv",
};

int main() {
    int file, i, j;
    float k, c;
    struct swarm *s;
    double pi;
    double sum = 0.0;
    clock_t start;
    clock_t end;
    float seconds;

    for (file = 1; file < 2; file++) {
        start = clock();

        s = initializeSwarm(100, file_path[file]);


        printf("Initial \n");
        for ( j = 0; j < LEN_SOL; j++) {
            printf(" %.5f ", s->global_best[j]);        
        }

        printf("\n");
        printf("Fitness: %lf \n", objectiveFunction(s->global_best));

        for (i = 0; i < MAX_ITER; i++) {
            //printf("\n Iteration %d \n", i);
            updateSwarm(s);
            //updateParameters(s, i);

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

        end = clock();
        seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Time in sec of run: %f", seconds);
    }

    
  
    return 0;
}