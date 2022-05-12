#pragma once
#include "PointSave.h"

void PointSave::addPoint(ToolSave& point)
{
    //assert (*point);
    points[currentLength].pos = point.pos;
    points[currentLength].size = point.size;
    points[currentLength].color = point.color;
    points[currentLength].thickness = point.thickness;
    currentLength++;
}

int PointSave::getByteSize()
{
    byteLength = sizeof(*this) + Length * sizeof(ToolSave);
    return byteLength;
}