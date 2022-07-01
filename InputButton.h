#pragma once
#include "DrawBibliothek.h"

struct InputButton : Window
{
    int* parameter = NULL;
    int parametrBeforeRedacting = NULL;
    int* minParametr = NULL;
    int* maxParametr = NULL;
    int cursorPos = 0;
    int lastTimeClicked = 0;
    int delta = 30;
    int lastTimeCursorConditionChanged = 0;
    bool shouldShowCursor = false;
    bool wasClicked = false;

    HCURSOR cursor = NULL;
    

    bool* confirmInput = NULL;

    COLORREF cadreColor = NULL;
    COLORREF cursorColor = NULL;
    

    InputButton(AbstractAppData* _app, Rect _rect, int* _parameter, int *_minParametr, int *_maxParametr, COLORREF _mainColor, COLORREF _cadreColor, COLORREF _cursorColor, bool* _confirmInput = NULL) :
        Window (_app, _rect, _mainColor),
        parameter (_parameter),
        cadreColor (_cadreColor),
        cursorColor (_cursorColor),
        minParametr(_minParametr),
        maxParametr(_maxParametr),
        confirmInput(_confirmInput)
    {
        needTransparencyOutput = true;

        cursorPos = getAmountOfNumbers(*parameter);

        cursor = LoadCursor(NULL, IDC_IBEAM);

    }

    int getAmountOfNumbers(int num);
    void checkKeyboard();
    void backSpace();
    void moveCursorLeft();
    void moveCursorRight();
    void drawCursor();



    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};
