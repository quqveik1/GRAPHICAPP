#pragma once
#include "LaysMenu.h"


void LaysMenu::onClick(Vector mp)
{
    mousePos = mp;
    if (advancedMode)
    {
        if (advancedMode && !isClickedLastTime())
        {
            clickHandle();

            if (canvasManager->getActiveCanvas() != NULL)
            {
                for (int i = 0; i < canvasManager->getActiveCanvas()->currentHistoryLength; i++)
                {
                    Rect button = { .pos = {(double)i, handle.rect.getSize().y + i * sectionHeight}, .finishPos = {rect.getSize().x, handle.rect.getSize().y + (i + 1) * sectionHeight} };
                    if (button.inRect(mp))
                    {
                        canvasManager->getActiveCanvas()->activeLayNum = i;
                    }
                }

                Rect addLayButton = { .pos = {0, rect.getSize().y - buttonSize.y}, .finishPos = {rect.getSize().x,  rect.getSize().y} };
                if (addLayButton.inRect(mp) && canvasManager->getActiveCanvas())
                {
                    canvasManager->getActiveCanvas()->createLay();
                }
            }
        }
    }
}

void LaysMenu::draw()
{
    app->setColor(color, finalDC);
    app->rectangle(0, 0, app->systemSettings->DCMAXSIZE, app->systemSettings->DCMAXSIZE, finalDC);
    char text[30] = {};

    handle.print(finalDC);
    controlHandle();

    rect.finishPos.y = handle.rect.getSize().y + rect.pos.y;
    if (canvasManager->getActiveCanvas() != NULL)
    {
        rect.finishPos.y = handle.rect.getSize().y + rect.pos.y + sectionHeight * canvasManager->getActiveCanvas()->currentLayersLength + buttonSize.y;

        for (int i = 0; i < (canvasManager->getActiveCanvas())->currentLayersLength; i++)
        {

            sprintf(text, "Слой %d", i + 1);

            app->setAlign(TA_CENTER, finalDC);
            app->setColor(app->systemSettings->TextColor, finalDC);
            app->selectFont("Arial", sectionFont, finalDC);

            app->drawText(sideThickness, sideThickness + handle.rect.getSize().y + sectionHeight * i, rect.getSize().x, handle.rect.getSize().y + sectionHeight * (i + 1), text, finalDC, DT_VCENTER);

            app->line(0, handle.rect.getSize().y + sectionHeight * (i), rect.getSize().x, handle.rect.getSize().y + sectionHeight * (i), finalDC);
        }
        app->bitBlt(finalDC, 0, rect.getSize().y - buttonSize.y, 0, 0, addNewLayButton);
        app->line(0, rect.getSize().y - buttonSize.y, rect.getSize().x, rect.getSize().y - buttonSize.y, finalDC);
    }
    setMbLastTime();
}
