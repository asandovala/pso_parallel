#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"

#define PI 3.14159265358979323846

double * getRangeOfData(double *data, int from, int to) {
  int i;
  double * copyData = (double *) malloc(sizeof(double *) * (to - from));
  for (i = from; i < to; i++) {
    copyData[i - from] = data[i];
    printf("%lf ", data[i]);    //TODO, QUE WEA...
  }

  return copyData;
}

double getAverageSine(double *data) {
  int i;
  int len = sizeof(data)/sizeof(data[0]);
  double sum = 0.0;
  for (i = 0; i < len; i++) {
    sum += sin(data[i]);    
  }

  return sum/len;
}

double getAverageCosine(double *data) {
  int i;
  int len = sizeof(data)/sizeof(data[0]);
  double sum = 0.0;
  for (i = 0; i < len; i++) {
    sum += cos(data[i]);    
  }

  return sum/len;
}

double getPrevailingDirection(double *data) {
  double uj;
  double sj = getAverageSine(data);
  double cj = getAverageCosine(data);
  int i;

  printf("Params: %lf %lf \n", sj, cj);
  printf("Data: \n");   
  int len = sizeof(data)/sizeof(data[0]);
  for (i = 0; i < len; i++) {
    printf("%lf", data[i]);    
  }
  printf("\n");

  if (sj >= 0 && cj > 0) {
    uj = atan(sj/cj);
  } else if (sj > 0 && cj == 0) {
    uj = PI * 0.5;   
  } else if (cj < 0) {
    uj = PI + atan(sj / cj);
  } else if (sj > 0 && cj == -1) {
    uj = PI;    
  } else if (sj < 0 && cj > 0) {
    uj = 2 * PI  + atan(sj / cj);
  } else if (sj < 0 && cj == 0) {
    uj = 3 * PI * 0.5;
  }

  return uj;
}

double getConcentration(double *data) {
  double sj = getAverageSine(data);
  double cj = getAverageCosine(data);
  return abs(23.29041409 - 16.8617370 * pow(pow(sj,2) + pow(cj, 2), 0.25) );
}

double getWeightAproximation(double *frequencies, int from, int to) {
  double numerator = 0.0;
  double denominator = 1.0;
  int i;
  int len = sizeof(frequencies)/sizeof(frequencies[0]);

  for (i = 0; i < len; i++) {
    denominator += frequencies[i];
    if (i >= from && i < to) {
       numerator += frequencies[i];
    }
  }

  return numerator/denominator;
}