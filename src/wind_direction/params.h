#ifndef PARAMS_CALCULATOR 
#define PARAMS_CALCULATOR

double * getRangeOfData(double *data, int from, int to);

double getAverageSine(double *data, int lenData);

double getAverageCosine(double *data, int lenData);

double getPrevailingDirection(double *data, int lenData);

double getConcentration(double *data, int lenData);

double getWeightAproximation(int from, int to);

#endif 