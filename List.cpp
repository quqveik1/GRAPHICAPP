#pragma once
#include "List.h"


void List::addNewItem(Window* openButton, HDC dc/* = NULL*/, const char* text/* = NULL*/)
{
    Rect newRect = { .pos = {0, (double)(currLen)*itemHeight}, .finishPos = {rect.getSize().x, (double)(currLen + 1) * itemHeight} };
    items[currLen]->rect = newRect;
    items[currLen]->color = app->systemSettings->MenuColor;
    items[currLen]->getOpeningManager() = (Manager*)openButton;
    items[currLen]->dc = dc;
    items[currLen]->text = text;
    items[currLen]->reInit();


    addWindow(items[currLen]);
}


void List::controlRect()
{
    rect.finishPos = { rect.pos.x + oneItemSize.x, rect.pos.y + oneItemSize.y * currLen };
}


List* List::addSubList(const char* ListText, int newListLength/* = NULL*/)
{
    if (!newListLength) newListLength = length;
    List* subList = new List(app, getNewSubItemCoordinats(), oneItemSize, newListLength);

    isThisItemList[currLen] = true;
    addNewItem(subList, NULL, ListText);

    return subList;
}

Vector List::getNewSubItemCoordinats()
{
    return Vector{ rect.finishPos.x, rect.pos.y + (double)(currLen)*itemHeight };
}




void List::draw()
{

    controlRect();
    standartManagerDraw(this);

    for (int i = 0; i < currLen; i++)
    {
        //pointers[i]->advancedMode = advancedMode;
        app->setColor(app->systemSettings->SecondMenuColor, finalDC, app->systemSettings->SIDETHICKNESS);
        app->line(0, i * itemHeight, rect.getSize().x, i * itemHeight, finalDC);

        if (items[i]->getOpeningManager()->advancedMode)
            app->ellipse(rect.getSize().x * 0.9 - activeItemCircleSize, ((double)i + 0.5) * itemHeight - activeItemCircleSize, rect.getSize().x * 0.9 + activeItemCircleSize, ((double)i + 0.5) * itemHeight + activeItemCircleSize, finalDC);

        if (isThisItemList[i] && !advancedMode)
        {
            items[i]->getOpeningManager()->advancedMode = false;
        }
    }

    if (!getMBCondition()) lastClickedItemNum = -1;
    setMbLastTime();
}

void List::onClick(Vector mp)
{
    mousePos = getMousePos();
    int clikedButtonNum = standartManagerOnClick(this, mp);
    if (clikedButtonNum >= 0 && clikedButtonNum != lastClickedItemNum && !isClickedLastTime())
    {
        //printf ("last: %d, current: %d\n", lastClickedItemNum, clikedButtonNum);
        if (pointers[clikedButtonNum]->advancedMode && mayFewWindowsBeOpenedAtTheSameTime)
        {
            clickButton(pointers[clikedButtonNum], this, mp);
        }
        lastClickedItemNum = clikedButtonNum;
    }
}

