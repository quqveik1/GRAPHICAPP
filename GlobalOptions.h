#pragma once
#include "..\Q_Vector.h"

const int DELTACLOCKTIME = 100;

//double SizeKForCloseCanvas = 0.05;
COLORREF MenuColor = RGB(45, 45, 45);
COLORREF TextColor = RGB(255, 255, 255);
COLORREF BackgroundColor = TX_BLACK;
COLORREF DrawColor = TX_RED;

int MainFont = 20;

const int HANDLEHEIGHT = 26;
const int MENUBUTTONSWIDTH = 50;
const int SIDETHICKNESS = 3;

int DrawingMode = 1;
const int TOOLSNUM = 5;

const Vector SCREENSIZE = { 1900, 900 };
const int DCMAXSIZE = 2000;
int test1 = 0 ;

//double IncomeBrightness = 255;
//double Brightness = 0;
double FirstFilterValue = 0;
double SecondFilterValue = 0;


bool confirmFilter = false;

RGBQUAD(*FilterAlgorithm)(RGBQUAD pixel, double FirstValue, double SecondValue);
