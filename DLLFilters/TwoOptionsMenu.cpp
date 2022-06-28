#pragma once
#include "TwoOptionsMenu.h"


void TwoOptionsMenu::onClick(Vector mp)
{
	clickHandle ();

	if (upSlider.rect.inRect(mp) && !isClickedLastTime())
	{
        clickButton (&upSlider, this, getMousePos());
		//upSlider.onClick(mp - upSlider.rect.pos);
	}
	if (downSlider.rect.inRect(mp) && !isClickedLastTime())
	{
        clickButton (&downSlider, this, getMousePos());
		//downSlider.onClick(mp - downSlider.rect.pos);
	}
	//if (closeButton.getAbsRect().inRect(mx, my) && !isClickedLastTime()) advancedMode = false;
	if (confirmButton.rect.inRect(mp) && !isClickedLastTime())
	{
		confirmFilter = true;
	}
   
}

void TwoOptionsMenu::draw()
{
    if (canvasManager && canvasManager->getActiveCanvas())
    {
        activeCanvas = canvasManager->getActiveCanvas(); 
        activeLay = activeCanvas->getActiveLay();
    }


    if (getMBCondition() == 2) 
    {
        //data->drawOnScreen(copyOfTempDC);
        _getch();
    }

    if (!getMBCondition() && (lastfirstVal != firstVal || lastsecondVal != secondVal))
    {
        useAlgorithm();
        lastfirstVal = firstVal;
        lastsecondVal = secondVal;
    }
    else
    {
    }
    

	controlHandle();

	if (dc) copyOnDC(0, 0, dc);
	upSlider.draw();
	//app->drawOnScreen(finalDC);
	app->bitBlt(finalDC, upSlider.rect.pos.x, upSlider.rect.pos.y, upSlider.rect.getSize().x, upSlider.rect.getSize().y, upSlider.finalDC);

	downSlider.draw();
	app->bitBlt(finalDC, downSlider.rect.pos.x, downSlider.rect.pos.y, downSlider.rect.getSize().x, downSlider.rect.getSize().y, downSlider.finalDC);
	char upNum[100] = {};
	char downNum[100] = {};
	sprintf(upNum, "%d", (int)firstVal);
	sprintf(downNum, "%d", (int)secondVal);

	txSetTextAlign(TA_LEFT, finalDC);
	//app->setColor(TX_BLACK, finalDC);
	txSelectFont ("Arial", 18, -1, FW_DONTCARE, false, false, false, 0, finalDC);
	txTextOut (185, 43, upNum, finalDC);
	txTextOut (185, 92, downNum, finalDC);

    bool nonConfirm = 0;


    if (confirmFilter)
    {
        if (canvasManager && canvasManager->getActiveCanvas() && canvasManager->getActiveCanvas()->finalDC)
        {
            apply ();
            firstVal = 0;
            secondVal = 0;
            confirmFilter = false;
        }
    }
}

