#pragma once

#include "DrawBibliothek.h"
#include "WindowsLib.cpp"
#include "CLay.h"
#include "MainTools.h"
#include "ZoneSizeControl.h"

struct Canvas : Manager
{
	HDC canvas = NULL;
    RGBQUAD *canvasArr = NULL;
	HDC tempFilterDC = NULL;
	RGBQUAD *tempFilterDCArr = NULL;
    char name[MAX_PATH] = {};

	bool nonConfirmFilter = false; // показывает есть ли сейчас непримененный фильтр
	bool reCountEnded = false;
	double lineThickness = 3;
	COLORREF drawColor = TX_RED;
	struct Window closeCanvas;
	bool clearBackground = true;
    Vector canvasCoordinats = {};

    const int LayersNum = 100;
	int currentLayersLength = 0;
	int activeLayNum = 0; 
    Vector laysSize = {};
    CLay* lay = new CLay[LayersNum]{};
    ToolLay* toolLays = new ToolLay[LayersNum]{};
    int currentToolLength = 0;
    bool editingMode = false;
    bool activeTool = false;
    int DrawingModeLastTime = 0;
    ProgrammeDate* currentDate = new ProgrammeDate({}, {}, {}, TX_WHITE);

    ZoneSizeControl zoneSizeControl;
    bool needFrameToWork = true;

	Canvas (AbstractAppData* _app, Rect _rect, const char* _name, HDC _closeDC = NULL) :
		Manager (_app, _rect, 5, true, NULL, {.pos = {0, 0}, .finishPos = {_rect.getSize ().x, _app->systemSettings->HANDLEHEIGHT}}),
		canvasCoordinats ({}),
        laysSize(_rect.getSize()),
		closeCanvas   (_app, { .pos = {_rect.getSize().x - _app->systemSettings->MENUBUTTONSWIDTH, 0}, .finishPos = {_rect.getSize().x, _app->systemSettings->HANDLEHEIGHT} }, TX_RED, _closeDC, this, "X"),
        DrawingModeLastTime (systemSettings->DrawingMode),
        zoneSizeControl (this, &rect, &needFrameToWork)
	{
        if (_name)strcpy(name, _name);
        addWindow (&closeCanvas);
	}

	void controlSize();
	void controlSizeSliders ();
	void saveHistory ();
	HDC playHistoryDC (int stepBack);
	void returnHistory(int stepsBack);
	void deleteHistory ();

    void drawCadre();

	void createLay ();
	bool controlLay ();
    void controlEditLay();
	void drawLay ();
    void cleanOutputLay();

	void controlFilter();

    void finishTool();
    void controlTool();
    void startTool();
    void changeTool();
    void initToolLay();
    void addToolLay();
    void setToolToToolLay(ToolLay* toollay);
    void setCurrentData();
   

    virtual HDC getImageForSaving();
    virtual CLay* getActiveLay();
    virtual int getEditingMode();
    virtual Vector getLaySize();
    virtual int getCurrentLayLength();
    virtual int getCurrentToolLengthOnActiveLay();
    virtual const char* getName();

    int getActiveLayNum();
    ToolLay* getNewToolLay();
    bool isDrawingModeChanged();
    Tool* getActiveTool();
    void setActiveToolLayNum(int num);
    int getLastNotStartedToolNum();

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
	virtual void deleteButton () override;
};