#pragma once
#include "Q_Vector.h"
#include <stdio.h>




struct CSystemSettings
{
    int WindowStyle = _txWindowStyle;

    const int DELTACLOCKTIME = 100;

    HWND MAINWINDOW = NULL;

    COLORREF MenuColor = NULL;
    COLORREF SecondMenuColor = NULL;
    COLORREF TextColor = NULL;
    COLORREF BackgroundColor = NULL;
    COLORREF DrawColor = NULL;
    COLORREF TRANSPARENTCOLOR = NULL;


    int ONEMENUBUTTONSNUM = NULL; //���������� ������� � ��������� MENU
    int ONELAYTOOLSLIMIT = NULL;


    int MainFont = NULL;
    char FONTNAME[MAX_PATH] = {};
    int TEXTFORMAT = NULL;// = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS;

    double HANDLEHEIGHT = NULL;
    double BUTTONWIDTH = NULL;
    double BUTTONHEIGHT = NULL;
    double MENUBUTTONSWIDTH = NULL;
    double SIDETHICKNESS = NULL;
    double SCROLLBARTHICKNESS = NULL;

    int debugMode = NULL;

    int DrawingMode = NULL;
    int TOOLSNUM = NULL;

    Vector SizeOfScreen = {};
    int DCMAXSIZE = NULL;
    Vector DCVECTORSIZE = {};

    const char* pathForSettings;
};



void setDefaultSystemSettings(CSystemSettings* systemSettings);

void setSystemSettings(CSystemSettings* systemSettings, const char* path);

void setColorSettings(FILE* ssFile, COLORREF* color, const char* name);
void setIntSettings(FILE* ssFile, int* integer, const char* name);
void setDoubleSettings(FILE* ssFile, double* integer, const char* name);
void setStringSettings(FILE* ssFile, char* str, const char* name);


void saveSystemSettings(CSystemSettings* systemSettings, const char* path);

void saveColorSettings(FILE* ssFile, COLORREF* color, const char* name);
void saveIntSettings(FILE* ssFile, int* integer, const char* name);
void saveDoubleSettings(FILE* ssFile, double* integer, const char* name);
void saveStringSettings(FILE* ssFile, char* str, const char* name);

