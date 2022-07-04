#pragma once
#include "MainManager.h"

void MainManager::draw()
{
    if (app->wasResized())
    {
        for (int i = 0; i < currLen; i++)
        {
            assert(pointers[i]);
            int posResult = 0;
            posResult = pointers[i]->rect.pos < app->systemSettings->SizeOfScreen;
            if (!posResult)
            {
                Vector delta = app->systemSettings->SizeOfScreen - app->systemSettings->lastTimeSizeOfScreen;
                if (pointers[i]->rect.pos.x < app->systemSettings->SizeOfScreen.x) delta.x = 0;
                if (pointers[i]->rect.pos.y < app->systemSettings->SizeOfScreen.y) delta.y = 0;
                pointers[i]->MoveWindow(delta);
            }
        }
    }
    standartManagerDraw(this);

    if (!app->isFullScreen())
    {
        app->drawCadre({ .pos = {0, -5}, .finishPos = app->systemSettings->SizeOfScreen }, finalDC, color, 5);
    }
}
