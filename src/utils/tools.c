#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include <assert.h>

/* Returns an integer in the range [0, n).
 * 
 * Uses rand(), and so is affected-by/affects the same seed.
 */
int randint(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        /* Chop off all of the values that would cause skew...*/
        long end = RAND_MAX / n; /* truncate skew */
        assert (end > 0L);
        end *= n;

        /* ... and ignore results from rand() that fall above that limit.
 	* (Worst case the loop condition should succeed 50% of the time,
 	* so we can expect to bail out of this loop pretty quickly.)*/
        int r;
        while ((r = rand()) >= end);
    
        return r % n;
    }
}

double rand01() {
    return (double)rand() / (double)RAND_MAX;
}

double rangeDoubleInRange(double min, double max) {
    assert(max > min);
    return rand01() * (max - min) + min;
}

/*int main() {
    for (int i = 0; i < 20; i++) {
        printf("Random 1-20: %f \n", rangeDoubleInRange(1.0, 20.0));
    }
    return 0;        
}*/
