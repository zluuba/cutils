#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>

#define RAND_INT(min, max) \
    ((min) + rand() % (((max) - (min)) + 1))

#define RAND_FLOAT(min, max) \
    ((float)(min) + ((float)((max) - (min))) * (rand() / (float)RAND_MAX))

#define RAND_BOOL(probability) \
    ((float)(probability) * (float)RAND_MAX > rand())

#endif // RANDOM_H
