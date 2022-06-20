#pragma once

#include "..\DrawBibliothek.h"
#include "..\Slider.cpp";
#include "..\..\Macroses.h"
#include "..\Canvas.h"
#include "..\TransferFilterStructure.h"
#include "..\CanvasManager.h"
#include "..\LoadManager.h"


struct TwoOptionsMenu : CFilter
{
	Slider upSlider;
	Slider downSlider;
	double firstVal = 0;
	double secondVal = 0;
    double lastfirstVal = 0;
	double lastsecondVal = 0;

    bool confirmFilter = false;


    CLay *activeLay = NULL;
    Lay *lastActiveLay = NULL;
    HDC copyOfTempDC = NULL; //делается копия, тк каждый раз основной холст копируется на временный


	Window confirmButton;
    CanvasManager *canvasManager;
    Canvas *activeCanvas = NULL;

    CLoadManager* loadManager;

    

	TwoOptionsMenu(CSystemSettings* _systemSettings, Rect _rect, Vector firstDomain, Vector secondDomain, AbstractAppData *_app = NULL)	:
		CFilter (_systemSettings, _rect, 3, _app, false, NULL, { .pos = {0, 0}, .finishPos = {getSize().x,_systemSettings->HANDLEHEIGHT} }),
		upSlider(_systemSettings, { .pos = {10, 65}, .finishPos = {165, 80}}, &firstVal, 0.3, _app->loadManager, firstDomain.x, firstDomain.y, true, true),
		downSlider(_systemSettings, { .pos = {10, 125}, .finishPos = {165, 140} }, &secondVal, 0.3, _app->loadManager, secondDomain.x, secondDomain.y, true, true),
		confirmButton(_systemSettings, { .pos = {240, 40}, .finishPos = {325, 60} }),
        canvasManager ((CanvasManager*)_app->canvasManager),
        loadManager (_app->loadManager)
	{
		addWindow(&upSlider);
		addWindow(&downSlider);
		addWindow(&confirmButton);
		compressImage(dc, getSize(), loadManager->loadImage("ContrastMenu.bmp"), {335, 179});
	}


    virtual void useAlgorithm () {};

	virtual void draw() override;
	virtual void onClick(Vector mp) override;
};