#ifndef FREQ_
#define FREQ_

#define NUMBER_OF_CLASSES 28

struct freq_data {
    double *rawData;
    double *classesFrequencies; 
    int len;
} DATA_DIRECTION;

/* 
 * Load freq data of some year and save in local memory, DATA_DIRECTION
 */
void loadFreqData();


/*
 * Return the relative frequency of the given direction, if no exist, return 0.
 */
double getRelFreq(double vel);

#endif
