#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freq/freq.h"
#include "params.h"

#define PI 3.14159265358979323846

double * getRangeOfData(double *data, int from, int to) {
  int i;
  double * copyData = (double *) malloc(sizeof(double *) * (to - from));
  for (i = from; i < to; i++) {
    copyData[i - from] = data[i];
  }

  return copyData;
}

double getAverageSine(double *data, int lenData) {
  int i;
  double sum = 0.0;
  for (i = 0; i < lenData; i++) {
    sum += sin(data[i]);    
  }

  return sum/lenData;
}

double getAverageCosine(double *data, int lenData) {
  int i;
  double sum = 0.0;
  for (i = 0; i < lenData; i++) {
    sum += cos(data[i]);    
  }

  return sum/lenData;
}

double getPrevailingDirection(double *data, int lenData) {
  double uj;
  double sj = getAverageSine(data, lenData);
  double cj = getAverageCosine(data, lenData);
  int i;

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

double getConcentration(double *data, int lenData) {
  double sj = getAverageSine(data, lenData);
  double cj = getAverageCosine(data, lenData);
  double f1 = 23.29041409;
  //double f1 = pow(pow(sj,2) + pow(cj, 2), 0.5);
  //double f1 = 23.29041409;
  //double f2 = pow(pow(sj,2) + pow(cj, 2), 1.5);
  double f2 = 16.8617370 * pow(pow(sj,2) + pow(cj, 2), 0.25);
  double f3 = 17.4749884 * exp(-(pow(sj,2) + pow(cj, 2)) );
  //double f3 = pow(sj,2) + pow(cj, 2);

  double result = 1.0 / fabs(f1 - f2 - f3);

  //double result = fabs(f1 - f2 );//- f3);
  //double result = fabs((2*f1 - f2)/(1 - f3));//- f3);

  if (result > 700.0) { //limit
    result = 700.0;
  }

  //printf("K stimated %lf f1: %lf f2: %lf f3: %lf \n", result, f1, f2, f3);

  //return fabs(f1 - f2);
  return result;
}

double getWeightAproximation(int from, int to) {
  double numerator = 0.0;
  double denominator = 0.0;
  double *frequencies = DATA_DIRECTION.classesFrequencies;
  int len = NUMBER_OF_CLASSES;
  int i;

  for (i = 0; i < len; i++) {
    denominator += frequencies[i];
    if (i >= from && i < to) {
       numerator += frequencies[i];
    }
  }

  return numerator/denominator;
}