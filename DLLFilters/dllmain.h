#pragma once

#include "..\..\Q_Vector.h"
#include "ContrastMenu.cpp"

//extern "C" COLORREF __declspec (dllexport) Plus30(COLORREF color);

extern "C" __declspec (dllexport) RGBQUAD  BrightnessFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

extern "C" __declspec (dllexport) RGBQUAD KontrastFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

extern "C" __declspec (dllexport) RGBQUAD BrightnessKontrastFilter(RGBQUAD pixel, double FirstValue, double SecondValue);


