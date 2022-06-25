#pragma once
#include "GlobalOptions.h"

int CSystemSettings::save(const char* path)
{
    FILE* ssFile = fopen(path, "wb");
    if (!ssFile)
    {
        printf("Файл не открылся\n");
        return 1;
    }

    int result = fwrite(this, sizeof(char), byteSize, ssFile);

    fclose(ssFile);
    return 0;
}


int CSystemSettings::read(const char* path)
{
    FILE* ssFile = fopen(path, "rb");
    if (!ssFile)
    {
        printf("Файл не открылся\n");
        return 1;
    }

    int result = fread(this, sizeof(char), byteSize, ssFile);

    fclose(ssFile);

    

    return 0;
}



void setDefaultSystemSettings(CSystemSettings* systemSettings)
{
    if (!systemSettings)
    {
        printf("systemSettings не существует дефолтные параметры не установились.\n");
        return;
    }
    systemSettings->MenuColor = RGB(45, 45, 45);
    systemSettings->SecondMenuColor = RGB(30, 30, 30);
    systemSettings->TextColor = RGB(255, 255, 255);
    systemSettings->BackgroundColor = RGB(0, 0, 0);
    systemSettings->DrawColor = RGB(128, 0, 0);
    systemSettings->TRANSPARENTCOLOR = RGB(57, 57, 57);

    systemSettings->ONEMENUBUTTONSNUM = 10;
    systemSettings->ONELAYTOOLSLIMIT = 100;

    systemSettings->MainFont = 20;
    strcpy (systemSettings->FONTNAME, "Arial");
    systemSettings->TEXTFORMAT = 262165;

    systemSettings->HANDLEHEIGHT = 26.000000;
    systemSettings->BUTTONWIDTH = 50.000000;
    systemSettings->BUTTONHEIGHT = 50.000000;
    systemSettings->MENUBUTTONSWIDTH = 50.000000;
    systemSettings->SIDETHICKNESS = 2.000000;
    systemSettings->SCROLLBARTHICKNESS = 20.000000;

    systemSettings->debugMode = 1;

    systemSettings->DrawingMode = 1;

    systemSettings->SizeOfScreen.x = 1900.000000;
    systemSettings->SizeOfScreen.y = 900.000000;
    systemSettings->FullSizeOfScreen = { .x = (double)GetSystemMetrics(SM_CXSCREEN), .y = (double)GetSystemMetrics(SM_CYSCREEN) };
    systemSettings->lastTimeSizeOfScreen = systemSettings->FullSizeOfScreen;

    systemSettings->WindowStyle = -2134376448;

    systemSettings->DCMAXSIZE = 1000;

    systemSettings->DCVECTORSIZE.x = 1000.000000;
    systemSettings->DCVECTORSIZE.y = 1000.000000;

}





void setSystemSettings(CSystemSettings* systemSettings, const char* path)
{
    assert(path);
    FILE* ssFile = fopen(path, "r");
    assert(ssFile);
    setIntSettings(ssFile, &systemSettings->MainFont,    "MainFont");
    setStringSettings(ssFile, systemSettings->FONTNAME, "FONTNAME");

    setIntSettings(ssFile, &systemSettings->debugMode, "debugMode");

    setIntSettings(ssFile, &systemSettings->DrawingMode, "DrawingMode");
    
    setIntSettings(ssFile, &systemSettings->WindowStyle, "WindowStyle");

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

        if (_color)*color = _color;
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

    int result = fscanf(ssFile, format, integer);

}

void setStringSettings(FILE* ssFile, char* str, const char* name)
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
    strcpy (str, finalStr);
}



int saveSystemSettings(CSystemSettings* systemSettings, const char* path)
{
    assert(path);
    FILE* ssFile = fopen(path, "w");
    if (!ssFile)
    {
        printf("Сохранение всех данных не прошло||Errno: %d\n", errno);
        return (int)ssFile; 
    }

    saveIntSettings(ssFile, &systemSettings->MainFont, "MainFont");
    saveStringSettings(ssFile, systemSettings->FONTNAME, "FONTNAME");

    saveIntSettings(ssFile, &systemSettings->debugMode, "debugMode");

    saveIntSettings(ssFile, &systemSettings->DrawingMode, "DrawingMode");

    saveIntSettings(ssFile, &systemSettings->WindowStyle, "WindowStyle");
    
    fclose(ssFile);

    return (int)ssFile;
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

void saveStringSettings(FILE* ssFile, char* str, const char* name)
{
    fprintf(ssFile, "%s = %s;\n", name, str);
}