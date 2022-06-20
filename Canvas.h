#pragma once

#include "DrawBibliothek.h"
#include "CLay.h"
#include "MainTools.h"

struct Canvas : Manager
{
	HDC canvas = NULL;
    RGBQUAD *canvasArr = NULL;
	HDC tempFilterDC;
	RGBQUAD *tempFilterDCArr = NULL;
	bool nonConfirmFilter = false; // показывает есть ли сейчас непримененный фильтр
	bool reCountEnded = false;
	double lineThickness = 3;
	COLORREF drawColor = TX_RED;
	struct Window closeCanvas;
	bool clearBackground = true;
	Vector canvasCoordinats;
	Vector canvasSize;
	
	//bool confirmFilter;

	Vector startResizingCursor = {};
	bool isResizing = false;
    Rect resizingPlace = {};

	Slider scrollBarVert;
	Slider scrollBarHor;
	bool wasClicked = false;
	Vector lastClick = {};
	double testNum = 0;

    const int LayersNum = 100;
	int currentLayersLength = 0;
	int activeLayNum = 0; 
    CLay* lay = new CLay[LayersNum]{};

    ToolLay* toolLays = new ToolLay[LayersNum]{};
    int currentToolLength = 0;

	const int HistoryLength = 1000;
	HDC lastSavedDC;
	CHistoryStep* history = new CHistoryStep[HistoryLength];
	int timeSavedHistory = 0;
	//HDC *history = new HDC[HistoryLength]{};
	int *historyOfDrawingMode = new int [HistoryLength]{};
	int currentHistoryNumber = 0;
	int currentHistoryLength = 0;
	int timesShowedHistoryInRow = 0;

    bool editingMode = false;

    bool activeTool = false;
    int DrawingModeLastTime;
    ProgrammeDate *currentDate = new ProgrammeDate ({}, {}, {}, TX_WHITE);

    CLoadManager* loadManager;


	Canvas (CSystemSettings* _systemSettings, Rect _rect, CLoadManager *_loadManager, HDC _closeDC = NULL) :
		Manager (_systemSettings, _rect, 5, true, NULL, {.pos = {0, 0}, .finishPos = {_rect.getSize ().x, _systemSettings->HANDLEHEIGHT}}),
		canvasCoordinats ({}),
		canvasSize (_systemSettings->DCVECTORSIZE),
		closeCanvas   (_systemSettings, { .pos = {_rect.getSize().x - _systemSettings->MENUBUTTONSWIDTH, 0}, .finishPos = {_rect.getSize().x, _systemSettings->HANDLEHEIGHT} }, TX_RED, _closeDC, this, "X"),
		scrollBarHor  (_systemSettings, {.pos = {5, _rect.getSize().y - _systemSettings->SCROLLBARTHICKNESS}, .finishPos = {_rect.getSize().x - _systemSettings->SCROLLBARTHICKNESS, _rect.getSize().y}}, &canvasCoordinats.x, 0.3, _loadManager, 0, 500, true, false),
		scrollBarVert (_systemSettings, {.pos = {_rect.getSize().x - _systemSettings->SCROLLBARTHICKNESS, _systemSettings->SCROLLBARTHICKNESS}, .finishPos = {_rect.getSize().x, _rect.getSize().y - _systemSettings->SCROLLBARTHICKNESS}}, &canvasCoordinats.y, 0.3, _loadManager, 0, 500, false, false),
        resizingPlace ({0, 0, 0.1 * rect.getSize().x, 0.1 * rect.getSize().y}),
        loadManager (_loadManager),
        DrawingModeLastTime (systemSettings->DrawingMode)
	{
		scrollBarVert.manager = this;
		scrollBarHor.manager = this;

        addWindow (&closeCanvas);
        addWindow (&scrollBarVert);
        addWindow (&scrollBarHor);
	}

	void controlSize();
	void controlSizeSliders ();
	void saveHistory ();
	HDC playHistoryDC (int stepBack);
	void returnHistory(int stepsBack);
	void deleteHistory ();

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
   

    HDC getImageForSaving();
    virtual CLay* getActiveLay();
    int getCurrentToolLengthOnActiveLay();
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