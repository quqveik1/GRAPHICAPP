#pragma once
#include "ToolsMenus.h"

void ToolsPalette::drawOneLine(int lineNum)
{
    pointers[lineNum]->draw();
    //app->drawOnScreen (pointers[lineNum]->dc);
    if (pointers[lineNum]->advancedMode) app->bitBlt(finalDC, pointers[lineNum]->rect.pos.x, pointers[lineNum]->rect.pos.y + handle.rect.finishPos.y, pointers[lineNum]->rect.finishPos.x, pointers[lineNum]->rect.finishPos.y, pointers[lineNum]->finalDC);

    if (app->systemSettings->DrawingMode - 1 == lineNum)
    {
        app->setColor(TX_WHITE, finalDC);
        app->rectangle(pointers[lineNum]->rect.pos.x, pointers[lineNum]->rect.pos.y + handle.rect.finishPos.y, pointers[lineNum]->rect.pos.x + pointers[lineNum]->rect.getSize().x * 0.1, pointers[lineNum]->rect.pos.y + pointers[lineNum]->rect.getSize().y * 0.1 + handle.rect.finishPos.y, finalDC);
    }

    app->setColor(TX_BLACK, finalDC);
    app->line(0, pointers[lineNum]->rect.pos.y + handle.rect.getSize().y, rect.getSize().x, pointers[lineNum]->rect.pos.y + handle.rect.getSize().y, finalDC);
}

int ToolsPalette::onClickLine(Vector mp)
{
    for (int lineNum = currentSize - 1; lineNum >= 0; lineNum--)
    {

        bool missClicked = false;
        if (pointers[lineNum]->rect.inRect(mp.x, mp.y))
        {
            setActiveWindow(pointers[lineNum]);
            clickButton(pointers[lineNum], this, mp);
            app->systemSettings->DrawingMode = lineNum + 1;
            lastSelected = lineNum;

            missClicked = false;

            if (pointers[lineNum]->advancedMode) return missClicked + 2;
        }
        else
        {
            missClicked = true;
        }


    }
    return  1;
}

void ToolMenu::onUpdate()
{
    Canvas* activeCanvas = canvasManager->getActiveCanvas();

    if (activeCanvas) currentSize = activeCanvas->getCurrentToolLengthOnActiveLay() + 1;
    rect.finishPos.y = currentSize * app->systemSettings->BUTTONHEIGHT + handle.rect.finishPos.y + rect.pos.y;
}


void ToolMenu::drawOneLine(int lineNum)
{
    if (!canvasManager->getActiveCanvas()) return;

    const char* nameOfTool = NULL;
    HDC toolDC = NULL;
    CLay* lay = canvasManager->getActiveCanvas()->getActiveLay();

    int notStartedNum = canvasManager->getActiveCanvas()->getLastNotStartedToolNum();
    if (lineNum == notStartedNum)
    {
        nameOfTool = "Новый инструмент";
        toolDC = emptyToolDC;
    }
    else
    {
        Tool* tool = lay->getToolLays()[lineNum]->getTool();

        assert(tool);

        toolDC = tool->getDC();
        nameOfTool = tool->getName();
    }

    lastSelected = lay->getActiveToolLayNum();


    char outputText[MAX_PATH] = {};
    sprintf(outputText, "%d - %s", lineNum + 1, nameOfTool);

    int linePosY = app->systemSettings->BUTTONHEIGHT * lineNum + handle.rect.finishPos.y;

    app->bitBlt(finalDC, 0, linePosY, app->systemSettings->BUTTONWIDTH, linePosY + app->systemSettings->BUTTONHEIGHT, toolDC);
    app->setColor(app->systemSettings->MenuColor, finalDC);
    app->rectangle(app->systemSettings->BUTTONWIDTH, linePosY, rect.getSize().x, linePosY + app->systemSettings->BUTTONHEIGHT, finalDC);

    app->setColor(app->systemSettings->TextColor, finalDC);
    app->drawText(app->systemSettings->BUTTONWIDTH, linePosY, rect.getSize().x, linePosY + app->systemSettings->BUTTONHEIGHT, outputText, finalDC, app->systemSettings->TEXTFORMAT);

    app->setColor(TX_BLACK, finalDC);
    app->line(0, linePosY, rect.getSize().x, linePosY, finalDC);

    if (lastSelected == lineNum)
    {
        app->setColor(TX_WHITE, finalDC);
        app->rectangle(0, linePosY, 5, linePosY + 5, finalDC);
    }
}


int ToolMenu::onClickLine(Vector mp)
{
    double my = mp.y;
    //int my = mp.y;

    int buttonNum = floor(my / app->systemSettings->BUTTONWIDTH);


    if (!(buttonNum >= 0 && buttonNum <= currentSize))  return 0;

    canvasManager->getActiveCanvas()->setActiveToolLayNum(buttonNum);

    return 1;
}

