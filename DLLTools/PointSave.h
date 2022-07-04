#pragma once
#include "..\Tool.h"
#include "..\DllSettings.h"

struct PointSave : ToolData
{
    ÑDllSettings* dllSettings;
    int Length;
    int currentLength = 0;
    Vector size = {};
    COLORREF color = {};
    Vector* pointsPosition = new Vector[Length]{};


    PointSave(ÑDllSettings* _dllSettings, int _Length = 0) :
          dllSettings (_dllSettings)
    {
        if (!_Length) Length = dllSettings->POINTSAVELENGTH;
    }

    void addPoint(Vector pos);
};