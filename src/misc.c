#include <time.h>
#include "../includes/misc.h"

float get_random(float a, float b) {
    float tmp = (rand() % 10001)/10000.0;
    return a + (b - a) * tmp;
}
