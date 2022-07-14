#pragma once
#include "StringButton2.h"


void StringButton2::moveCursorLeft()
{
    if (cursorPos - 1 >= 0) cursorPos--;
    lastTimeClicked = clock();
}

void StringButton2::moveCursorRight()
{
    if (cursorPos + 1 <= currentTextSize) cursorPos++;
    lastTimeClicked = clock();
}

void StringButton2::shiftTextForward(char* _text, int startPos, int finishPos)
{
    for (int i = finishPos; i >= startPos; i--)
    {
        if (finishPos < maxTextSize)
        {
            text[i + 1] = text[i];
        }
    }
}

void StringButton2::shiftTextBack(char* _text, int startPos, int finishPos)
{
    for (int i = startPos; i <= finishPos; i++)
    {
        if (i - 1 >= 0)
        {
            text[i - 1] = text[i];
        }
    }

}

int StringButton2::getTextSize(char* _text)
{
    int i = 0;
    while (_text[i] != NULL)
    {
        i++;
    }
    return i;
}

bool StringButton2::isSymbolAllowed(int symbol)
{
    if (0x30 <= symbol && symbol <= 0x7E)
    {
        return true; 
    }

    //КИРИЛИЦА
    if (0xC0 <= symbol && symbol <= 0xFF)
    {
        return true;
    }
    //знаки препинания
    if (0xBB <= symbol && symbol <= 0xDF)
    {
        return true;
    }

    if (symbol == VK_SPACE)
    {
        return true;
    }
    return false;
}


void StringButton2::draw()
{
    doBeforeMainBlock();

    if (needToShow)
    {
        app->setColor(color, finalDC);
        app->rectangle({ 0, 0 }, getSize(), finalDC);

        if (rect.inRect(getAbsMousePos()))
        {
            app->setCursor(cursor);
        }

        if (getInputMode())
        {
            if (getActiveWindow() != this || app->getAsyncKeyState(VK_RETURN))
            {
                getInputMode() = 0;
                confirmEnter();
            }

            if (app->getAsyncKeyState(VK_ESCAPE))
            {
                getInputMode() = 0;
                if (textBeforeRedacting)
                {
                    strcpy(text, textBeforeRedacting);
                }

            }

            checkKeyboard();

            drawCursor();
            text[currentTextSize] = 0;
        }
        else
        {
            cursorPos = getTextSize(text);
        }

        char parametrString[MAX_PATH] = {};
        sprintf(parametrString, "%s", text);

        char output[MAX_PATH] = {};

        modifyOutput(output, parametrString);

        app->selectFont("Roboto", font, finalDC, fontSizeX);
        app->setColor(app->systemSettings->TextColor, finalDC);
        app->drawText(deltaAfterCadre, 0, getSize().x, getSize().y, output, finalDC, DT_VCENTER);

        Rect cadreRect = { .pos = {0, 0}, .finishPos = {getSize().x, getSize().y} };
        app->drawCadre(cadreRect, finalDC, cadreColor, 2);
    }

    doAfterMainBlock();

    setMbLastTime();
}

void StringButton2::modifyOutput(char* outputStr, char* originalStr)
{
    assert(outputStr);
    assert(originalStr);

    sprintf(outputStr, "%s", originalStr);
}


void StringButton2::onClick(Vector mp)
{
    setActiveWindow(this);
    if (!isClickedLastTime())
    {
        if (getInputMode())
        {
            double trueMP = mp.x - deltaAfterCadre;
            if (!isEqual(fontSizeX, 0))
            {
                int potentialCursorPos = trueMP / (fontSizeX);
                if (potentialCursorPos <= currentTextSize)
                {
                    cursorPos = potentialCursorPos;
                }
            }
        }

        if (!getInputMode())
        {
            if (textBeforeRedacting) delete textBeforeRedacting;
            textBeforeRedacting = new char[maxTextSize];
            assert(textBeforeRedacting);
            strcpy(textBeforeRedacting, text);
        }
        getInputMode() = 1;
        


    }
}



void StringButton2::backSpace()
{
    if (cursorPos > 0)
    {
        text[cursorPos - 1] = NULL;
        shiftTextBack(text, cursorPos, currentTextSize - 1);
        cursorPos--;
        currentTextSize--;
    }

}

void StringButton2::checkKeyboard()
{

    if (app->getAsyncKeyState(VK_RIGHT) && clock() - lastTimeClicked > delta)
    {
        moveCursorRight();
        return;
    }

    if (app->getAsyncKeyState(VK_LEFT) && clock() - lastTimeClicked > delta)
    {
        moveCursorLeft();
        return;
    }

    if (app->getAsyncKeyState(VK_BACK) && clock() - lastTimeClicked > delta)
    {
        backSpace();
        return;
    }

    if (!_kbhit())	return;
    int symbol = _getch();
   
    if (!isSymbolAllowed(symbol))
    {                    
        return;
    }

    if (currentTextSize < maxTextSize)
    {
        shiftTextForward(text, cursorPos, currentTextSize - 1);
        text[cursorPos] = symbol;
        cursorPos++;
        currentTextSize++;
    }

    lastTimeClicked = clock();

}

bool StringButton2::isAnyNormalKeyButtonClicked()
{
    // буквы да цифры
    for (int i = 0x30; i < 0x5A; i++)
    {
        if (app->getAsyncKeyState(i))
        {
            return true;
        }
    }
    //знаки препинания
    for(int i = 0xBB; i < 0xDF; i++)
    {
        if (app->getAsyncKeyState(i))
        {
            return true;
        }
    }

    if (app->getAsyncKeyState(VK_SPACE))
    {
        return true;
    }
    return false;
}

int StringButton2::findClickedKey()
{
    for (int i = 0; i <= 0xFF; i++)
    {
        if (app->getAsyncKeyState(i))
        {
            return i;
        }
    }

    return 0;
}

void StringButton2::drawCursor()
{
    int clockMS = clock();

    if (clockMS - lastTimeCursorConditionChanged > 500)
    {
        if (shouldShowCursor == false) shouldShowCursor = true;
        else shouldShowCursor = false;
        lastTimeCursorConditionChanged = clock();
    }

    if (!shouldShowCursor)  return;
    int tempCursorPos = cursorPos;
    int cursorXPosition = deltaAfterCadre + fontSizeX * tempCursorPos;
    if (tempCursorPos > 0)
    {
        //cursorXPosition += spaceBetween2Symbols * (tempCursorPos - 1);
    }
    app->setColor(cursorColor, finalDC);
    app->line(cursorXPosition, 0, cursorXPosition, getSize().y, finalDC);


}