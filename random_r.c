#define _POSIX_SOURCE
#include <stdlib.h>
#include <math.h>
#include "random_r.h"

/* 
    Successive calls to randomGaussian produce integer return values
    having a gaussian distribution with the given mean and standard
    deviation.  Return values may be negative.
*/

int randomGaussian_r(int mean, int stddev, unsigned int *state)
{
    double mu = 0.5 + (double)mean;
    double sigma = fabs((double)stddev);
    double f1 = sqrt(-2.0 * log((double)rand_r(state) / (double)RAND_MAX));
    double f2 = 2.0 * 3.14159265359 * (double)rand_r(state) / (double)RAND_MAX;
    if (rand_r(state) & (1 << 5))
        return (int)floor(mu + sigma * cos(f2) * f1);
    else
        return (int)floor(mu + sigma * sin(f2) * f1);
}
