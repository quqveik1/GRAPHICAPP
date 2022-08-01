#pragma once
#include "..\Tool.h"
#include "..\DllSettings.h"

struct PointSave : ToolData
{
    ÑDllSettings* dllSettings = NULL;
    int currentMaxLength = 0;
    int currentLength = 0;
    int pointsAddedByOneTime = 0;
    Vector size = {};
    COLORREF color = {};
    POINT* pointsPosition = NULL;


    PointSave(ÑDllSettings* _dllSettings, int _Length = 0) :
        dllSettings (_dllSettings),
        currentMaxLength (_Length),
        pointsAddedByOneTime (dllSettings->POINTSAVELENGTH)
    {
        if (!_Length) currentMaxLength = dllSettings->POINTSAVELENGTH;
        pointsPosition = new POINT[currentMaxLength]{};
    }

    void saveData(COLORREF currColor, Vector currSize);

    void addPoint(Vector pos);
};