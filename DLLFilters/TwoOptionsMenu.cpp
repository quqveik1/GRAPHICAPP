#pragma once
#include "TwoOptionsMenu.h"


void TwoOptionsMenu::onClick(Vector mp)
{
    mousePos = mp;
	clickHandle ();

	if (upSlider.rect.inRect(mp) && !isClicked)
	{
        clickButton (&upSlider, this, mousePos);
		//upSlider.onClick(mp - upSlider.rect.pos);
	}
	if (downSlider.rect.inRect(mp) && !isClicked)
	{
        clickButton (&downSlider, this, mousePos);
		//downSlider.onClick(mp - downSlider.rect.pos);
	}
	//if (closeButton.getAbsRect().inRect(mx, my) && !isClicked) advancedMode = false;
	if (confirmButton.rect.inRect(mp) && !isClicked)
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


    if (clicked == 2) 
    {
        //data->drawOnScreen(copyOfTempDC);
        _getch();
    }

    if (!clicked && (lastfirstVal != firstVal || lastsecondVal != secondVal))
    {
        useAlgorithm();
        lastfirstVal = firstVal;
        lastsecondVal = secondVal;
    }
    else
    {
    }
    

	controlHandle();
    controlMouse ();

	if (dc) copyOnDC(0, 0, dc);
	upSlider.draw();
	//printBlt(finalDC);
	txBitBlt(finalDC, upSlider.rect.pos.x, upSlider.rect.pos.y, upSlider.rect.getSize().x, upSlider.rect.getSize().y, upSlider.finalDC);

	downSlider.draw();
	txBitBlt(finalDC, downSlider.rect.pos.x, downSlider.rect.pos.y, downSlider.rect.getSize().x, downSlider.rect.getSize().y, downSlider.finalDC);
	char upNum[100] = {};
	char downNum[100] = {};
	sprintf(upNum, "%d", (int)firstVal);
	sprintf(downNum, "%d", (int)secondVal);

	txSetTextAlign(TA_LEFT, finalDC);
	//txSetAllColors(TX_BLACK, finalDC);
	txSelectFont ("Arial", 18, -1, FW_DONTCARE, false, false, false, 0, finalDC);
	txTextOut (185, 43, upNum, finalDC);
	txTextOut (185, 92, downNum, finalDC);

    bool nonConfirm = 0;


    if (confirmFilter)
    {
        if (canvasManager && canvasManager->activeWindow && canvasManager->activeWindow->finalDC)
        {
            apply ();
            firstVal = 0;
            secondVal = 0;
            confirmFilter = false;
        }
    }
}

