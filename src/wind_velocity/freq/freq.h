#ifndef FREQ_
#define FREQ_

//float **vel_freq;

struct freq_data {
    double *data;
    int len;
} DATA_DIRECTION;

/* 
 * Load freq data of some year and save in local memory, vel_freq
 */
void loadFreqData();


/*
 * Return the relative frequency of the given velocity, if no exist, return 0.
 */
float getRelFreq(float vel);

#endif
