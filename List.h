#pragma once
#include "OpenManager.h"

struct List : Manager
{
    int itemHeight;
    OpenManager** items;
    Vector oneItemSize;
    bool* isThisItemList;
    int lastClickedItemNum = -1;
    bool mayFewWindowsBeOpenedAtTheSameTime;
    int activeItemCircleSize = 3;

    List(AbstractAppData* _app, Vector _pos, Vector _oneItemSize, int _maxLength, bool _mayFewWindowsBeOpenedAtTheSameTime = true) :
        Manager(_app, { .pos = _pos, .finishPos = {_pos.x + _oneItemSize.x, _pos.y + _maxLength * _oneItemSize.y } }, _maxLength, false),
        mayFewWindowsBeOpenedAtTheSameTime(_mayFewWindowsBeOpenedAtTheSameTime),
        oneItemSize(_oneItemSize),
        itemHeight(systemSettings->HANDLEHEIGHT)
    {
        items = new OpenManager * [length];
        for (int i = 0; i < length; i++)  items[i] = new OpenManager(app);

        isThisItemList = new bool[length] {};
    }

    void addNewItem(Window* openButton, HDC dc = NULL, const char* text = NULL);
    Vector getNewSubItemCoordinats();
    List* addSubList(const char* ListText, int length = NULL);
    void controlRect();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};