#pragma once

#include "..\WindowsLib.cpp"
#include "..\Q_Vector.h"
#include "KontrastMenu.cpp"
#include "..\TransferFilterStructure.h"
#include "..\Canvas.h"
#include "BrightnessFilter.cpp"
#include "..\CanvasManager.h"

extern "C" __declspec (dllexport) DLLExportData * initDLL(AbstractAppData * data);

RGBQUAD  BrightnessFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

RGBQUAD KontrastFilter(RGBQUAD pixel, double Brightness, double IncomeBrightness);

RGBQUAD BrightnessKontrastFilter(RGBQUAD pixel, double FirstValue, double SecondValue);



  

