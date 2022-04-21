#pragma once
#include "ContrastMenu.h"


void ContrastMenu::onClick(Vector mp)
{
    mousePos = mp;
    //printf ("It is working");
   
	//if (!advancedMode) return;
	//int my = getRelativeMousePos().y;

    //int absCoordinats = getAbsCoordinats().y;
    //int absCoordinatsx = getAbsCoordinats().x;
    //int mrealY = txMouseY();
    //int mrealX = txMouseX();
    //Vector WindowMousePos = windowMousePos (false);

    //txSetAllColors (TX_RED, finalDC);
    //txRectangle (0, 0, 100, 100, finalDC);

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
		//brightnessSlider.maxNum = Brightness;
		//brightnessSlider.maxNum = 255;
		//FirstFilterValueSlider.maxNum = FirstFilterValue;
	}
   
}

void ContrastMenu::draw()
{

	//if (!advancedMode) return;

	if (txMouseButtons() != 1 && manager->activeWindow == this)
	{
		SecondFilterValue = brightness;
		FirstFilterValue = contrast;
	}

	controlHandle();
    controlMouse ();
    //printf ("Was drawn");

	if (dc) copyOnDC(0, 0, dc);
	upSlider.draw();
	//printBlt(finalDC);
	txBitBlt(finalDC, upSlider.rect.pos.x, upSlider.rect.pos.y, upSlider.rect.getSize().x, upSlider.rect.getSize().y, upSlider.finalDC);

	downSlider.draw();
	//printBlt(FirstFilterValueSlider.finalDC);
	txBitBlt(finalDC, downSlider.rect.pos.x, downSlider.rect.pos.y, downSlider.rect.getSize().x, downSlider.rect.getSize().y, downSlider.finalDC);
	//txBitBlt (FirstFilterValueSlider.rect.pos.x, FirstFilterValueSlider.rect.pos.y, FirstFilterValueSlider.finalDC);
	//txBitBlt (300, 300, FirstFilterValueSlider.finalDC);
	char upNum[100] = {};
	char downNum[100] = {};
	sprintf(upNum, "%d", (int)brightness);
	sprintf(downNum, "%d", (int)contrast);

	txSetTextAlign(TA_LEFT, finalDC);
	txSetAllColors(TX_BLACK, finalDC);
	txSelectFont ("Arial", 18, -1, FW_DONTCARE, false, false, false, 0, finalDC);
	txTextOut (185, 43, upNum, finalDC);
	txTextOut (185, 92, downNum, finalDC);

	if (manager->activeWindow == this)
	{
		FilterAlgorithm = algorithm;
	}
     //txSetAllColors (TX_RED, finalDC);
    //txRectangle (0, 0, 100, 100, finalDC);

	//txSetFillColor(TX_RED);
	//txRectangle(300, 300, 400, 400);
}