#pragma once
#include "PointSave.h"

void PointSave::addPoint(Vector pos)
{
    pointsPosition[currentLength] = pos;
    currentLength++;
}
