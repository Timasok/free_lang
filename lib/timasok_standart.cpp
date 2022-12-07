#include <math.h>
#include <stdio.h>

#include "timasok_standart.h"

const double EPS = 0.00001;

bool equals(const double num_1, const double num_2)
{
    if (abs(num_1 - num_2) < EPS)
        return true;

    return false;

}

bool equals(const int num_1, const int num_2)
{
    return (num_1 == num_2);

}