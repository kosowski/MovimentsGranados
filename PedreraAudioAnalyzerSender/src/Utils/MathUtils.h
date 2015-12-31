//
// Created by Miquel Àngel Soler on 31/12/15.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "math.h"

float truncateFloat(float value, int numDecimals)
{
    float divisor = powf(10, numDecimals);
    return roundf(value * divisor) / divisor;
}

#endif //MATHUTILS_H
