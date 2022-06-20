#pragma once
#include "GlobalOptions.h"

void setSystemSettings(CSystemSettings* systemSettings, const char* path)
{
    assert(path);
    systemSettings->pathForSettings = path;
    FILE* ssFile = fopen(path, "r");
    assert(ssFile);

    setColorSettings(ssFile, &systemSettings->MenuColor,       "MenuColor");
    setColorSettings(ssFile, &systemSettings->SecondMenuColor, "SecondMenuColor");
    setColorSettings(ssFile, &systemSettings->TextColor,       "TextColor");
    setColorSettings(ssFile, &systemSettings->BackgroundColor, "BackgroundColor ");
    setColorSettings(ssFile, &systemSettings->DrawColor,       "DrawColor");
    setColorSettings(ssFile, &systemSettings->TRANSPARENTCOLOR,"TRANSPARENTCOLOR");

    setIntSettings(ssFile, &systemSettings->ONEMENUBUTTONSNUM, "ONEMENUBUTTONSNUM");
    setIntSettings(ssFile, &systemSettings->ONELAYTOOLSLIMIT,  "ONELAYTOOLSLIMIT");

    setIntSettings(ssFile, &systemSettings->MainFont,    "MainFont");
    setStringSettings(ssFile, &systemSettings->FONTNAME, "FONTNAME");
    setIntSettings(ssFile, &systemSettings->TEXTFORMAT,  "TEXTFORMAT");

    setDoubleSettings(ssFile, &systemSettings->HANDLEHEIGHT,       "HANDLEHEIGHT");
    setDoubleSettings(ssFile, &systemSettings->BUTTONWIDTH,        "BUTTONWIDTH");
    setDoubleSettings(ssFile, &systemSettings->BUTTONHEIGHT,       "BUTTONHEIGHT");
    setDoubleSettings(ssFile, &systemSettings->MENUBUTTONSWIDTH,   "MENUBUTTONSWIDTH");
    setDoubleSettings(ssFile, &systemSettings->SIDETHICKNESS,      "SIDETHICKNESS");
    setDoubleSettings(ssFile, &systemSettings->SCROLLBARTHICKNESS, "SCROLLBARTHICKNESS");

    setIntSettings(ssFile, &systemSettings->debugMode, "debugMode");

    setIntSettings(ssFile, &systemSettings->DrawingMode, "DrawingMode");

    setDoubleSettings(ssFile, &systemSettings->SizeOfScreen.x, "SizeOfScreen.x");
    setDoubleSettings(ssFile, &systemSettings->SizeOfScreen.y, "SizeOfScreen.y");
    setIntSettings(ssFile, &systemSettings->WindowStyle, "WindowStyle");

    setIntSettings(ssFile, &systemSettings->DCMAXSIZE, "DCMAXSIZE");

    setDoubleSettings(ssFile, &systemSettings->DCVECTORSIZE.x, "DCVECTORSIZE.x");
    setDoubleSettings(ssFile, &systemSettings->DCVECTORSIZE.y, "DCVECTORSIZE.y");


    fclose(ssFile);
}


void setColorSettings(FILE* ssFile, COLORREF* color, const char* name)
{
    char format[MAX_PATH] = {};

    sprintf(format, " %s = %%s ", name);

    char data[MAX_PATH] = {};
    fscanf(ssFile, format, data);

    int resultOfComparision = strcmp(data, "RGB");

    if (resultOfComparision >= 0)
    {
        int red = 0;
        int green = 0;
        int blue = 0;
        fscanf(ssFile, " (%d, %d, %d); ", &red, &green, &blue);
        *color = RGB(red, green, blue);
    }
    else
    {
        COLORREF _color = 0;

        for (int i = 0; ; i++)
        {
            int number = (data[i] - '0');
            if (abs(number) > 9) break;

            _color = _color * 10 + number;
        }

        *color = _color;
    }
}

void setIntSettings(FILE* ssFile, int* integer, const char* name)
{
    char format[MAX_PATH] = {};
    sprintf(format, " %s = %%d; ", name);

    fscanf(ssFile, format, integer);

}

void setDoubleSettings(FILE* ssFile, double* integer, const char* name)
{
    char format[MAX_PATH] = {};
    sprintf(format, " %s = %%lf; ", name);

    fscanf(ssFile, format, integer);

}

void setStringSettings(FILE* ssFile, const char** str, const char* name)
{
    char format[MAX_PATH] = {};
    sprintf(format, " %s = %%s; ", name);

    char finalStr[MAX_PATH] = {};
    fscanf(ssFile, format, finalStr);

    for (int i = 0; i < MAX_PATH; i++)
    {
        if (finalStr[i] == ';') 
        {
            finalStr[i] = NULL;
            break;
        }
    }
    *str = finalStr;
}



void saveSystemSettings(CSystemSettings* systemSettings, const char* path)
{
    assert(path);
    systemSettings->pathForSettings = path;
    FILE* ssFile = fopen(path, "w");
    assert(ssFile);

    saveColorSettings(ssFile, &systemSettings->MenuColor, "MenuColor");
    saveColorSettings(ssFile, &systemSettings->SecondMenuColor, "SecondMenuColor");
    saveColorSettings(ssFile, &systemSettings->TextColor, "TextColor");
    saveColorSettings(ssFile, &systemSettings->BackgroundColor, "BackgroundColor ");
    saveColorSettings(ssFile, &systemSettings->DrawColor, "DrawColor");
    saveColorSettings(ssFile, &systemSettings->TRANSPARENTCOLOR, "TRANSPARENTCOLOR");

    saveIntSettings(ssFile, &systemSettings->ONEMENUBUTTONSNUM, "ONEMENUBUTTONSNUM");
    saveIntSettings(ssFile, &systemSettings->ONELAYTOOLSLIMIT, "ONELAYTOOLSLIMIT");

    saveIntSettings(ssFile, &systemSettings->MainFont, "MainFont");
    saveStringSettings(ssFile, &systemSettings->FONTNAME, "FONTNAME");
    saveIntSettings(ssFile, &systemSettings->TEXTFORMAT, "TEXTFORMAT");

    saveDoubleSettings(ssFile, &systemSettings->HANDLEHEIGHT, "HANDLEHEIGHT");
    saveDoubleSettings(ssFile, &systemSettings->BUTTONWIDTH, "BUTTONWIDTH");
    saveDoubleSettings(ssFile, &systemSettings->BUTTONHEIGHT, "BUTTONHEIGHT");
    saveDoubleSettings(ssFile, &systemSettings->MENUBUTTONSWIDTH, "MENUBUTTONSWIDTH");
    saveDoubleSettings(ssFile, &systemSettings->SIDETHICKNESS, "SIDETHICKNESS");
    saveDoubleSettings(ssFile, &systemSettings->SCROLLBARTHICKNESS, "SCROLLBARTHICKNESS");

    saveIntSettings(ssFile, &systemSettings->debugMode, "debugMode");

    saveIntSettings(ssFile, &systemSettings->DrawingMode, "DrawingMode");

    saveDoubleSettings(ssFile, &systemSettings->SizeOfScreen.x, "SizeOfScreen.x");
    saveDoubleSettings(ssFile, &systemSettings->SizeOfScreen.y, "SizeOfScreen.y");
    saveIntSettings(ssFile, &systemSettings->WindowStyle, "WindowStyle");

    saveIntSettings(ssFile, &systemSettings->DCMAXSIZE, "DCMAXSIZE");

    saveDoubleSettings(ssFile, &systemSettings->DCVECTORSIZE.x, "DCVECTORSIZE.x");
    saveDoubleSettings(ssFile, &systemSettings->DCVECTORSIZE.y, "DCVECTORSIZE.y");

    fclose(ssFile);
}


void saveColorSettings(FILE* ssFile, COLORREF* color, const char* name)
{
    int red = txExtractColor(*color, TX_RED);
    int green = txExtractColor(*color, TX_GREEN);
    int blue = txExtractColor(*color, TX_BLUE);
    fprintf (ssFile, "%s = RGB (%d, %d, %d);\n", name, red, green, blue);
}

void saveIntSettings(FILE* ssFile, int* integer, const char* name)
{
    fprintf(ssFile, "%s = %d;\n", name, *integer);
}

void saveDoubleSettings(FILE* ssFile, double* integer, const char* name)
{
    fprintf(ssFile, "%s = %lf;\n", name, *integer);
}

void saveStringSettings(FILE* ssFile, const char** str, const char* name)
{
    fprintf(ssFile, "%s = %s;\n", name, *str);
}