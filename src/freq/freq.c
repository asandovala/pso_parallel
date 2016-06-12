#include<stdio.h>
#include<stdlib.h>
#include "freq.h"

char file_path[] = "data/2015/parse/freq_data_15.csv";

void loadFreqData() {
    FILE *fp;
    char ch;    
    int lines = 0, i = 0, j;
    float d1, d2;

    fp = fopen(file_path, "r");

    //coun lines in file
    while(!feof(fp)) {
        ch = fgetc(fp);
        if (ch == '\n') {
            lines++;
        }
    }
    
    //set memory to data
    vel_freq.data = (float **) malloc(sizeof(float *) * lines);
    for (j = 0; j < lines; j++) {
        vel_freq.data[j] = (float*) malloc(sizeof(float *) * 2); 
    }
    vel_freq.len = lines;

    rewind(fp);

    //get data line by line
    while (fscanf(fp, "%f,%f", &d1, &d2) != EOF) {
        vel_freq.data[i][0] = d1; 
        vel_freq.data[i][1] = d2;
        i++; 
    }
       
    fclose(fp);    
}

float getRelFreq(float vel) {
    int lenData = vel_freq.len;
    int i;
    for (i = 0; i < lenData; i++) {
        if (vel_freq.data[i][0] == vel) {
            return vel_freq.data[i][1];
        }
    }

    return 0.0;
}

/*
int main() {
    loadFreqData();
    printf("value 9.0: %.2f \n", getRelFreq(9.0));
    printf("value 12.0: %.2f \n", getRelFreq(12.0));
    printf("value 3.0: %.2f \n", getRelFreq(3.0));
  
    return 0;
}
*/
