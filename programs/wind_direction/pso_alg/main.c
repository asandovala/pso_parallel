#include <stdio.h>
#include <stdlib.h>
#include "pso.h"
#include "freq/freq.h"
#include <time.h>

char *filePath[] = {
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

int repetitionExperiments = 10;	
int amountFiles = 1; //can't exceed the length of filePath

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
    int file, i, j;
    float k, c;
    struct swarm *s;
    double pi;
    double sum = 0.0;
    clock_t start;
    clock_t mid;
    clock_t end;
    float seconds;
    int exp;
    double fitness;

    double results[30];
    double bestResult;
    double worstResult;
    double meanResult; 

    double iterations[30];
    double bestIteration;
    double worstIteration;
    double meanIterations;

    double times[30];
    double bestTime;
    double worstTime;
    double meanTime;

    for (file = 0; file < amountFiles; file++) { //For All files
        printf("\n Current file: %s \n", filePath[file]);

        for (exp = 0; exp < repetitionExperiments; exp++) { //Repetition of experiment
            srand(exp + 1); 

            start = clock();

            s = initializeSwarm(PARTICLES, filePath[file]);

            /*
            printf("Initial \n");
            for ( j = 0; j < LEN_SOL; j++) {
                printf(" %.5f ", s->global_best[j]);        
            }

            printf("\n");
            printf("Fitness: %lf \n", objectiveFunction(s->global_best));*/

            for (i = 0; i < MAX_ITER; i++) {
                //printf("\n Iteration %d \n", i);
                updateSwarm(s);
                //updateParameters(s, i);

                if (objectiveFunction(s->global_best) < 22.362) {
                    break;
                }

                mid = clock();
                seconds = (float)(end - start) / CLOCKS_PER_SEC; 
                if (seconds > 1800) {
                    break;
                }
            }

            //printf("\n Iterations #: %d \n", i);
            iterations[exp] = i;

            /*for ( j = 0; j < LEN_SOL; j++) {
                printf(" %.5f,", s->global_best[j]);        
            }
            printf("\n");*/

            fitness = objectiveFunction(s->global_best);
            //printf("Fitness: %lf \n", fitness);
            results[exp] = fitness;

            end = clock();
            seconds = (float)(end - start) / CLOCKS_PER_SEC;
            //printf("Time in sec of run: %f", seconds);
            times[exp] = seconds;    
        }

         //Review experiments
        printf("\n Resumen de experimentos \n");

        printf("\n Results: \n");
        showArrayResults(results);
        printf("\n Iterations: \n");
        showArrayResults(iterations);
        printf("\n Times: \n");
        showArrayResults(times);

        bestResult = getMinValue(results);
        worstResult = getMaxValue(results);
        meanResult = getMeanValue(results); 

        bestIteration = getMinValue(iterations);
        worstIteration = getMaxValue(iterations);
        meanIterations = getMeanValue(iterations);

        bestTime = getMinValue(times);
        worstTime = getMaxValue(times);
        meanTime = getMeanValue(times);

        printf("\nBest Fitness  %lf \n", bestResult);
        printf(" Worst Fitness  %lf \n", worstResult);
        printf(" Mean Fitness  %lf \n", meanResult);

        printf("\nBest Iteration  %lf \n", bestIteration);
        printf(" Worst Iteration  %lf \n", worstIteration);
        printf(" Mean Iteration  %lf \n", meanIterations);

        printf("\nBest Time  %lf \n", bestTime);
        printf(" Worst Time  %lf \n", worstTime);
        printf(" Mean Time  %lf \n", meanTime);

        printf("\n ------ \n");

    }

    
  
    return 0;
}
