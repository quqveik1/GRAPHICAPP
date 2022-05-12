#pragma once
struct PointSave : ToolData
{
    int Length = POINTSAVELENGTH;
    int currentLength = 0;
    ToolSave* points = new ToolSave[Length]{};


    PointSave(int _Length = POINTSAVELENGTH) :
        Length(_Length)
    {
    }

    void addPoint(ToolSave& point);

    virtual int getByteSize();
};