#pragma once

#include "..\..\Q_Vector.h"
#include "ContrastMenu.cpp"
#include "..\TransferStructure.h"

//extern "C" COLORREF __declspec (dllexport) Plus30(COLORREF color);

extern "C" __declspec (dllexport) RGBQUAD  BrightnessFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

extern "C" __declspec (dllexport) RGBQUAD KontrastFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

extern "C" __declspec (dllexport) RGBQUAD BrightnessKontrastFilter(RGBQUAD pixel, double FirstValue, double SecondValue);

extern "C" __declspec (dllexport) Window *createContrastMenu (TransferData data, Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue));


