#ifndef FREQ_
#define FREQ_

#define NUMBER_OF_CLASSES 36

struct freq_data {
    double *rawData;
    double *classesFrequencies; 
    double sumAllFreq;
    int len;
} DATA_DIRECTION;

/* 
 * Load freq data of some year and save in local memory, DATA_DIRECTION
 */
void loadFreqData();


/*
 * Return the relative frequency of the given direction, if no exist, return 0.
 */
void getRelFreq(double *vel);

#endif
