#pragma once
#include "InputButton2.h"


int InputButton2::getIntFromText(char* _text, int textSize/* = 0 */)
{
    int resultNum = 0;
    if (!textSize) textSize = strlen(_text);

    for (int i = 0; i < textSize; i++)
    {
        if (!_text[i]) break;

        resultNum += (_text[i] - '0') * pow(10, textSize - i - 1);
    }

    return resultNum;

}



bool InputButton2::isSymbolAllowed(int symbol)
{
    int currentInt = getIntFromText (text, currentTextSize);
    if (symbol == '0' || currentInt == 0 || cursorPos == 0)
    {
        return false;
    }
    if ('0' <= symbol && symbol <= '9')
    {
        return true;
    }
    return false;
}


void InputButton2::modifyOutput(char* outputStr, char* originalStr)
{
    assert(outputStr && originalStr);
    if (text[0] == '0')
    {
        sprintf(outputStr, "");
        return;
    }

    if (mode == 1)
    {
        sprintf(outputStr, "%s%%", originalStr);
        return;
    }

    sprintf(outputStr, "%s", originalStr);

    
}



void InputButton2::confirmEnter()
{
    int numBeforeRedacting = getIntFromText(textBeforeRedacting, 0);
    int numAfterRedacting = getIntFromText(text, 0);
    if (numBeforeRedacting != numAfterRedacting && confirmInput)
    {
        *confirmInput = true;
    }
}

void InputButton2::doBeforeMainBlock()
{
    sprintf(text, "%d", *parameter);
}

void InputButton2::doAfterMainBlock()
{
    *parameter = getIntFromText(text, currentTextSize);
}
