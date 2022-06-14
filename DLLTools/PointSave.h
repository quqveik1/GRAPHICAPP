#pragma once
struct PointSave : ToolData
{
    int Length = POINTSAVELENGTH;
    int currentLength = 0;
    Vector size = {};
    COLORREF color = {};
    Vector* pointsPosition = new Vector[Length]{};


    PointSave(int _Length = POINTSAVELENGTH) :
        Length(_Length)
    {
    }

    void addPoint(Vector pos);
};