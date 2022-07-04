#pragma once

#include "..\Q_Vector.h"
#include "KontrastMenu.cpp"
#include "..\TransferFilterStructure.h"
#include "..\Canvas.h"
#include "BrightnessFilter.cpp"
#include "..\CanvasManager.h"

//extern "C" COLORREF __declspec (dllexport) Plus30(COLORREF color);

RGBQUAD  BrightnessFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

RGBQUAD KontrastFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

RGBQUAD BrightnessKontrastFilter(RGBQUAD pixel, double FirstValue, double SecondValue);

extern "C" __declspec (dllexport) DLLExportData *initDLL (AbstractAppData *data);

CFilter* createKontrastFilter();
CFilter* createBrightnessFilter();


  

