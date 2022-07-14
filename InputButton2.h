#pragma once
#include "StringButton2.cpp"
#include "cmath"


struct InputButton2 : StringButton2
{
    int* parameter = NULL;
    int* minParametr = NULL;
    int* maxParametr = NULL;

    bool* confirmInput = NULL;
    int mode = 0;
    char numText[MAX_PATH] = {};


    InputButton2(AbstractAppData* _app, Rect _rect, int* _parameter, int* _minParametr, int* _maxParametr, int _mode/* = 0*/, COLORREF _mainColor, COLORREF _cadreColor/* = RGB(144, 144, 144)*/, COLORREF _cursorColor/* = RGB(200, 200, 200)*/, bool* _confirmInput/* = NULL*/) :
        StringButton2(_app, _rect, NULL, MAX_PATH, _mainColor, _cadreColor, _cursorColor),
        parameter(_parameter),
        confirmInput (_confirmInput)
    {
        text = numText;
    }

    int getIntFromText(char* text, int textSize = 0);

    virtual bool isSymbolAllowed(int symbol);
    virtual void modifyOutput(char* outputStr, char* originalStr);
    virtual void confirmEnter();
    virtual void doBeforeMainBlock();
    virtual void doAfterMainBlock();
};