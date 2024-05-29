//
// Created by Francesco on 18/05/2024.
//

#ifndef ARKAN0_1_FUNCTIONS_H
#define ARKAN0_1_FUNCTIONS_H

#endif //ARKAN0_1_FUNCTIONS_H

#pragma once

#include <cmath>

float getCenter(float parentDimension, float childDimension) {
    return (parentDimension / 2) - (childDimension / 2);
}

double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
