#pragma once
#include "PointSave.h"

void PointSave::addPoint(Vector pos)
{
    if (currentLength >= dllSettings->POINTSAVELENGTH - 1) return;
    pointsPosition[currentLength] = pos;
    currentLength++;
}
