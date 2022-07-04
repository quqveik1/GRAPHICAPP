#pragma once
#include "cmath"

double deltaOfComparision = 0.00001;

bool isEqual (const double& a, const double& b);
bool isBigger(const double& a, const double& b);
bool isSmaller(const double& a, const double& b);



bool isEqual(const double& a, const double& b)
{
    if (fabs(a - b) < deltaOfComparision) return true;
    return false;
}

bool isBigger(const double& a, const double& b)
{
    if (a - b > deltaOfComparision) return true;
    else                            return false;

}

bool isSmaller(const double& a, const double& b)
{
    if (b - a > deltaOfComparision) return true;
    else                            return false;

}