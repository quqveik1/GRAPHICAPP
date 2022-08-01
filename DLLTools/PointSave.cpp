#pragma once
#include "PointSave.h"

void PointSave::addPoint(Vector pos)
{
    assert(dllSettings);
    if (currentLength >= currentMaxLength)
    {
        POINT* newPoints = new POINT[currentMaxLength = currentMaxLength + pointsAddedByOneTime]{};
        if (!memcpy(newPoints, pointsPosition, currentMaxLength * sizeof (POINT)))
        {
            delete newPoints;
            return;
        }

        delete pointsPosition;
        pointsPosition = newPoints;
    }
    pointsPosition[currentLength].x = std::lround(pos.x);
    pointsPosition[currentLength].y = std::lround(pos.y);
    currentLength++;
}

void PointSave::saveData(COLORREF currColor, Vector currSize)
{
    color = currColor;
    size = currSize;
}
