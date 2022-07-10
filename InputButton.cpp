#pragma once
#include "InputButton.h"



int InputButton::getAmountOfNumbers(int num)
{
    int answer = 1;
    while ((num /= 10) > 0) answer++;

    return answer;
}

void InputButton::moveCursorLeft()
{
    if (cursorPos - 1 >= 0 && *parameter != 0) cursorPos--;
    lastTimeClicked = clock();
}

void InputButton::moveCursorRight()
{
    if (cursorPos + 1 <= getAmountOfNumbers(*parameter) && *parameter != 0) cursorPos++;
    lastTimeClicked = clock();
}

void InputButton::backSpace()
{
    int newNum = *parameter;
    if (newNum == 0 || cursorPos <= 0) return;
    int numLength = getAmountOfNumbers(*parameter);;
    newNum /= pow(10, numLength - cursorPos + 1);

    int numsAfterEnteredNum = *parameter % ((int)pow(10, numLength - cursorPos));

    int numsAfterEnteredNumLength = numLength - cursorPos;

    newNum *= pow(10, numsAfterEnteredNumLength);
    newNum += numsAfterEnteredNum;

    if (newNum >= 0)
    {
        *parameter = newNum;
        cursorPos--;
    }


}

void InputButton::checkKeyboard()
{
    if (clock() - lastTimeClicked < 100) return;

    if (app->getAsyncKeyState (VK_RIGHT)) moveCursorRight();
    if (app->getAsyncKeyState(VK_LEFT)) moveCursorLeft();

    if (!_kbhit())	return;
    int symbol = _getch();

    if (symbol == VK_BACK)
    {
        backSpace();
    }
    

    if (!(symbol >= 48 && symbol <= 57)) return;

    int newNum = *parameter;
    int numLength = getAmountOfNumbers(*parameter);
    newNum /= pow(10, numLength - cursorPos);

    newNum *= 10;
    newNum += symbol - 48;


    int numsAfterEnteredNum = *parameter % ((int)pow(10, numLength - cursorPos));

    int numsAfterEnteredNumLength = numLength - cursorPos;

    newNum *= pow(10, numsAfterEnteredNumLength);
    newNum += numsAfterEnteredNum;


    if (newNum <= *maxParametr && newNum >= *minParametr)
    {
        if (*parameter != 0)cursorPos++;
        *parameter = newNum;
    }

    lastTimeClicked = clock();

}

void InputButton::drawCursor()
{
    int clockMS = clock();

    if (clockMS - lastTimeCursorConditionChanged > 500)
    {
        if (shouldShowCursor == false) shouldShowCursor = true;
        else shouldShowCursor = false;
        lastTimeCursorConditionChanged = clock();
    }

    if (!shouldShowCursor)  return;
    app->setColor(cursorColor, finalDC);
    int fontSize = 24 * 0.45;
    int tempCursorPos = cursorPos;
    if (*parameter == 0) tempCursorPos = 0;
    app->line(deltaAfterCadre + fontSize * tempCursorPos, 0, deltaAfterCadre + fontSize * tempCursorPos, getSize().y, finalDC);

    
}

void InputButton::draw()
{
    app->setColor(color, finalDC);
    app->rectangle({ 0, 0 }, getSize(), finalDC);


    char output[MAX_PATH] = {};

    int result = sprintf(output, "%d", *parameter);

    if (rect.inRect(getAbsMousePos()))
    {
        app->setCursor(cursor);
    }


    if (wasClicked)
    {
        if (getActiveWindow() != this || app->getAsyncKeyState(VK_RETURN))
        {
            wasClicked = false;
            if (*parameter != parametrBeforeRedacting && confirmInput) *confirmInput = true;
        }

        if (app->getAsyncKeyState(VK_ESCAPE))
        {
            wasClicked = false;
            *parameter = parametrBeforeRedacting;
        }

        if (*parameter == 0)
        {
            cursorPos = 1;
            result = sprintf(output, " ");
        }

        drawCursor();

        checkKeyboard();
        
    }
    else
    {
        cursorPos = getAmountOfNumbers(*parameter);
    }

    app->selectFont(app->systemSettings->FONTNAME, 24, finalDC);
    app->setColor(app->systemSettings->TextColor, finalDC);
    app->drawText(deltaAfterCadre, 0, getSize().x, getSize().y, output, finalDC, DT_VCENTER);

    Rect cadreRect = { .pos = {0, 0}, .finishPos = {getSize().x, getSize().y} };
    app->drawCadre(cadreRect, finalDC, cadreColor, 2);

    setMbLastTime();
}


void InputButton::onClick(Vector mp)
{
    setActiveWindow(this);
    if (!isClickedLastTime())
    {
        wasClicked = true;
        parametrBeforeRedacting = *parameter;
    }
}