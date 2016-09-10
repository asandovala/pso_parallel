#ifndef PARAMS_CALCULATOR 
#define PARAMS_CALCULATOR

double * getRangeOfData(double *data, int from, int to);

double getAverageSine(double *data);

double getAverageCosine(double *data);

double getPrevailingDirection(double *data);

double getConcentration(double *data);

double getWeightAproximation(double *data, int from, int to);

#endif 