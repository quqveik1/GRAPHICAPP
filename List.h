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
    double deltaAfterTextBeforeFrame = 50;

    bool needToHideAfterClick = false;

    List(AbstractAppData* _app, Vector _pos, Vector _oneItemSize, int _maxLength, bool _mayFewWindowsBeOpenedAtTheSameTime = true, bool _needToHideAfterClick = false) :
        Manager(_app, { .pos = _pos, .finishPos = {_pos.x + _oneItemSize.x, _pos.y + _maxLength * _oneItemSize.y } }, _maxLength, false, NULL, {}, (RGB(60, 60, 60))),
        mayFewWindowsBeOpenedAtTheSameTime(_mayFewWindowsBeOpenedAtTheSameTime),
        oneItemSize(_oneItemSize),
        itemHeight(systemSettings->HANDLEHEIGHT),
        needToHideAfterClick (_needToHideAfterClick)
    {
        items = new OpenManager * [length];
        for (int i = 0; i < length; i++)  items[i] = new OpenManager(app);

        isThisItemList = new bool[length] {};

        font = oneItemSize.y - 6;

    }

    ~List()
    {
        for (int i = 0; i < length; i++)  delete items[i];
        delete items;
    }

    void addNewItem(Window* openButton, HDC dc = NULL, const char* text = NULL, int* option = NULL, int keybind = NULL);
    Vector getNewSubItemCoordinats();
    List* addSubList(const char* ListText, int length = NULL);
    void controlRect();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};