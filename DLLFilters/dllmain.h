#pragma once

#include "..\Q_Vector.h"
#include "KontrastMenu.cpp"
#include "..\TransferFilterStructure.h"
#include "..\Canvas.h"
#include "BrightnessFilter.cpp"

//extern "C" COLORREF __declspec (dllexport) Plus30(COLORREF color);

extern "C" __declspec (dllexport) RGBQUAD  BrightnessFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

extern "C" __declspec (dllexport) RGBQUAD KontrastFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

extern "C" __declspec (dllexport) RGBQUAD BrightnessKontrastFilter(RGBQUAD pixel, double FirstValue, double SecondValue);

extern "C" __declspec (dllexport) Window *createContrastMenu (Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue), Manager *canvasManager);

extern "C" __declspec (dllexport) DLLExportData *initDLL (AbstractAppData *data);

extern "C" __declspec (dllexport) CFilter* createKontrastFilter();

extern "C" __declspec (dllexport) CFilter* createBrightnessFilter();


  

