#include <stdio.h>
#include <stdlib.h>
#include "freq.h"
#include <errno.h>
#include <string.h>

char file_path[] = "../../data/2015/parse/direction_freq_data_15.csv"; //Data must be sorted, min to max

void loadFreqData() {
    FILE *fp;
    char ch;    
    int lines = 0, i = 0;
    double value, decimals;

    fp = fopen(file_path, "r");

    if (!fp) {
        printf("Error open file.\n");
        printf("Msj: %s\n", strerror(errno));
    }

    //count lines in file
    while(!feof(fp)) {
        ch = fgetc(fp);
        if (ch == '\n') {
            lines++;
        }
    }
    
    //set memory to data
    DATA_DIRECTION.rawData = (double *) malloc(sizeof(double *) * lines + 1);
    DATA_DIRECTION.classesFrequencies = (double *) malloc(sizeof(double *) * NUMBER_OF_CLASSES);
    DATA_DIRECTION.len = lines;

    rewind(fp);

    //get data line by line
    while (fscanf(fp, "%lf", &value) != EOF) {
        DATA_DIRECTION.rawData[i] = value;
        i++; 
    }

    setDensity(DATA_DIRECTION.classesFrequencies);

    double n = 0.0;
    for (i = 0; i < NUMBER_OF_CLASSES; i++) {
        n += DATA_DIRECTION.classesFrequencies[i];
    }
    DATA_DIRECTION.sumAllFreq = n;    

       
    fclose(fp);
}

void setDensity(double *classesFrequencies) {
    int lenData = DATA_DIRECTION.len;
    int i,j;
    double *classes = (double *) malloc(sizeof(double *) * NUMBER_OF_CLASSES);
    double count = 0.0;
    double PI = 3.14159265358979323846;
    double data;
    double lenClass = (2*PI/NUMBER_OF_CLASSES);

    for (i = 0; i < NUMBER_OF_CLASSES; i++) {
        classes[i] = (i + 1) * (2.0 * PI /NUMBER_OF_CLASSES);
    }

    printf("Prob \n");
    for (i = 0; i < NUMBER_OF_CLASSES; i++) {

        for (j = 0; j < lenData; j++) {
            data = DATA_DIRECTION.rawData[j];
            if (i == 0) {
                if (data >= 0 && data < classes[i]) {
                    count += 1.0;
                }
            } else {
                if (data >= classes[i - 1] && data < classes[i]) {
                    count += 1.0;
                }
            }
        }

        classesFrequencies[i] = count;
        //Pruebas con densidad
        //classesFrequencies[i] = (count/lenData) / lenClass;
        count = 0.0;

        printf(" %lf ", classesFrequencies[i]);
    }
    printf("\n");
}