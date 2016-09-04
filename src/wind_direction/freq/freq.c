#include <stdio.h>
#include <stdlib.h>
#include "freq.h"

char file_path[] = "../../../data/2015/parse/direction_freq_data_15.csv";

void loadFreqData() {
    FILE *fp;
    char ch;    
    int lines = 0, i = 0, j;
    double value;

    fp = fopen(file_path, "r");
    if (!fp) {
        printf("Error file path.");
    }

    //coun lines in file
    while(!feof(fp)) {
        ch = fgetc(fp);
        if (ch == '\n') {
            lines++;
        }
    }
    
    //set memory to data
    DATA_DIRECTION.rawData = (double *) malloc(sizeof(double *) * lines);
    DATA_DIRECTION.classesFrequencies = (double *) malloc(sizeof(double *) * lines);
    DATA_DIRECTION.len = lines;

    rewind(fp);

    //get data line by line
    while (fscanf(fp, "%f", &value) != EOF) {
        DATA_DIRECTION.rawData[i] = value;
        i++; 
    }

    for (i = 0; i < lines;i++) {
        DATA_DIRECTION.classesFrequencies[i] = getRelFreq(DATA_DIRECTION.rawData[i]);
    }
       
    fclose(fp);
}

double getRelFreq(double direction) {
    int lenData = DATA_DIRECTION.len;
    int i,j;
    int *classes = (int *) malloc(sizeof(int *) * NUMBER_OF_CLASSES);
    double count = 0.0;
    double PI = 3.14159265358979323846;

    for (i = 0; i < NUMBER_OF_CLASSES; i++) {
        classes[i] = i * (2/NUMBER_OF_CLASSES);
    }

    for (i = 0; i < lenData; i++) {

        for (j = 0; j < NUMBER_OF_CLASSES; j++) {

            if (DATA_DIRECTION.rawData[i] <= classes[i] * PI) {
                count += 1.0;
            }

            break;
        }

    }

    return count / lenData;
}
