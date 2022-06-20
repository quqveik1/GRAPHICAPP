#pragma once
#include "Q_Vector.h"


struct CSystemSettings
{

    const int DELTACLOCKTIME = 100;

    HWND MAINWINDOW = NULL;

    //double SizeKForCloseCanvas = 0.05;
    COLORREF MenuColor = RGB(45, 45, 45);
    COLORREF SecondMenuColor = RGB(30, 30, 30);
    COLORREF TextColor = RGB(255, 255, 255);
    COLORREF BackgroundColor = TX_BLACK;
    COLORREF DrawColor = TX_RED;
    const COLORREF TRANSPARENTCOLOR = RGB(57, 57, 57);


    const int ONEMENUBUTTONSNUM = 10; //количество пунктов в структрах MENU
    const int ONELAYTOOLSLIMIT = 100;


    int MainFont = 20;
    const char* FONTNAME = "Arial";
    const int TEXTFORMAT = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS;

    const double HANDLEHEIGHT = 26;
    const double BUTTONWIDTH = 50;
    const double BUTTONHEIGHT = 50;
    const double MENUBUTTONSWIDTH = 50;
    const double SIDETHICKNESS = 2;
    const double SCROLLBARTHICKNESS = 20;

    int debugMode = 1;


    int GummiThickness = 10;

    int DrawingMode = 1;
    const int TOOLSNUM = 5;

    const Vector SCREENSIZE = { 1900, 900 };
    const int DCMAXSIZE = 1000;
    const Vector DCVECTORSIZE = { 1000, 1000 };
    int test1 = 0;
};
