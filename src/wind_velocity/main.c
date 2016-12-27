#include <stdio.h>
#include <stdlib.h>
#include "pso.h"
#include <time.h>

char *file_path[] = {
    "../../data/2015/parse/freq_data_15.csv",
    "../../data/2014/parse/freq_data_14.csv",
    "../../data/2013/parse/freq_data_13.csv",
    "../../data/All/parse/freq_data_All.csv",
    "../../data/2015/parse/freq_data_15_ene-mar.csv",
    "../../data/2015/parse/freq_data_15_abr-jun.csv",
    "../../data/2015/parse/freq_data_15_jul-sep.csv",
    "../../data/2015/parse/freq_data_15_oct-dic.csv",
};

double getMaxValue(double *values) {
    double max = -999999999;
    int i;

    for (i = 0; i < 30; i++) {
        if (max == -999999999 || max < values[i]) {
            max = values[i];        
        }
    }

    return max;
}

double getMinValue(double *values) {
    double min = 999999999;
    int i;

    for (i = 0; i < 30; i++) {
        if (min == 999999999 || min > values[i]) {
            min = values[i];        
        }
    }

    return min;
}

double getMeanValue(double *values) {
    double sum = 0;
    double mean;
    int i;

    for (i = 0; i < 30; i++) {     
        sum += values[i];
    }

    mean = sum/30.0;

    return mean;
}

void showArrayResults(double *values) {
    int i;

    printf("\n > ");
    for (i = 0; i < 30; i++) {     
        printf(" %lf ,", values[i]);
    }
    printf("< \n");
}

int main() {
    int file, i, exp;
    float k, c;
    struct swarm *s;
    clock_t start;
    clock_t end;
    float seconds;

    double resultsK[30];
    double resultsC[30];
    double meanResultK; 
    double meanResultC; 

    for (file = 0; file < 8; file++) {
        printf("\n ------ \n");

        printf("\n File %s \n", file_path[file]);    

        for (exp = 0; exp < 30; exp++) { //Repetition of experiment
            srand(exp + 1); 

            start = clock();

            s = initializeSwarm(50, PARAM_W, PARAM_C1, PARAM_C2, file_path[file]);

            for (i = 0; i < MAX_ITER; i++) {
               updateSwarm(s);
    	       updateParameters(s,i);
            }

            k = s->global_best[0];
            c = s->global_best[1];

            resultsK[exp] = k;
            resultsC[exp] = c;

            end = clock();
            seconds = (float)(end - start) / CLOCKS_PER_SEC;
            //printf("Time in sec of run: %f", seconds);
        }

        printf("\n Resumen de experimentos \n");

        meanResultK = getMeanValue(resultsK); 
        meanResultC = getMeanValue(resultsC); 

        printf("\n Todos los K \n");
        showArrayResults(resultsK);

        printf("\n Todos los C \n");
        showArrayResults(resultsC);

        printf(" Mean Result K  %lf \n", meanResultK);
        printf(" Mean Result C  %lf \n", meanResultC);

        printf("\n ------ \n");
             
    }

    return 0;
}