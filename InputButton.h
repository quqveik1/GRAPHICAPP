#pragma once
#include "DrawBibliothek.h"

struct InputButton : Window
{
    int* parameter = NULL;
    int parametrBeforeRedacting = NULL;
    int minParametr = 0;
    int maxParametr = 255;
    int cursorPos = 0;
    int lastTimeClicked = 0;
    int delta = 30;
    int lastTimeCursorConditionChanged = 0;
    bool shouldShowCursor = false;
    bool wasClicked = false;
    bool confirmed = false;

    COLORREF cadreColor = NULL;
    COLORREF cursorColor = NULL;
    

    InputButton(AbstractAppData* _app, Rect _rect, int* _parameter, COLORREF _mainColor, COLORREF _cadreColor, COLORREF _cursorColor) :
        Window (_app, _rect, _mainColor),
        parameter (_parameter),
        cadreColor (_cadreColor),
        cursorColor (_cursorColor)
    {
        needTransparencyOutput = true;

        cursorPos = getAmountOfNumbers(*parameter);
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
