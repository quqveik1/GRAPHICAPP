#pragma once
#include "DrawBibliothek.h"

struct MainManager : Manager
{
    MainManager(AbstractAppData* _app, Rect _rect, int _length) :
        Manager(_app, _rect, _length)
    {
    }

    virtual void draw() override;
};