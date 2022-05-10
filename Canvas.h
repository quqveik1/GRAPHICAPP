#pragma once

#include "DrawBibliothek.h"
#include "Q_Filter.h"

struct Canvas : Manager
{
	HDC canvas = NULL;
    RGBQUAD *canvasArr = NULL;
	HDC tempFilterDC;
	RGBQUAD *tempFilterDCArr = NULL;
	bool nonConfirmFilter = false; // ���������� ���� �� ������ ������������� ������
	bool reCountEnded = false;
	double lineThickness = 3;
	COLORREF drawColor = TX_RED;
	Window closeCanvas;
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

	int lastFirstFilterValue = FirstFilterValue;
	int lastSecondFilterValue = SecondFilterValue;
	int lastRecountFirstFilterValue = FirstFilterValue;
	int lastRecountSecondFilterValue = FirstFilterValue;//������� ��� ������� ����� �������
	DWORD lastTimeRecountFilter = GetTickCount();

	const int LayersNum = 10;
	Lay *lay = new Lay[LayersNum];
	Vector cursorPos = {};
	int currentLayersLength = 0;
	int activeLayNum = 0;   

	const int HistoryLength = 1000;
	HDC lastSavedDC;
	CHistoryStep* history = new CHistoryStep[HistoryLength];
	int timeSavedHistory = 0;
	//HDC *history = new HDC[HistoryLength]{};
	int *historyOfDrawingMode = new int [HistoryLength]{};
	int currentHistoryNumber = 0;
	int currentHistoryLength = 0;
	int timesShowedHistoryInRow = 0;

    //Tool** tools = new Tool*[10];
    bool activeTool = false;
    ProgrammeDate *currentDate = new ProgrammeDate ({}, {}, {}, TX_WHITE);


	Canvas (Rect _rect, HDC _closeDC = NULL) :
		Manager (_rect, 5, true, NULL, {.pos = {0, 0}, .finishPos = {_rect.getSize ().x, HANDLEHEIGHT}}),
		canvasCoordinats ({}),
		canvasSize({DCMAXSIZE, DCMAXSIZE}),
		closeCanvas ({ .pos = {_rect.getSize().x - MENUBUTTONSWIDTH, 0}, .finishPos = {_rect.getSize().x, HANDLEHEIGHT} }, TX_RED, _closeDC, this, "X"),
		scrollBarHor ({.pos = {5, _rect.getSize().y - SCROLLBARTHICKNESS}, .finishPos = {_rect.getSize().x - SCROLLBARTHICKNESS, _rect.getSize().y}}, &canvasCoordinats.x, 0.3, 0, 500, true, false),
		scrollBarVert ({.pos = {_rect.getSize().x - SCROLLBARTHICKNESS, SCROLLBARTHICKNESS}, .finishPos = {_rect.getSize().x, _rect.getSize().y - SCROLLBARTHICKNESS}}, &canvasCoordinats.y, 0.3, 0, 500, false, false),
        resizingPlace ({0, 0, 0.1 * rect.getSize().x, 0.1 * rect.getSize().y})
	{
		//canvas = txCreateDIBSection (canvasSize.x, canvasSize.y, &canvasArr);
		scrollBarVert.manager = this;
		scrollBarHor.manager = this;

		//tempFilterDC = txCreateDIBSection (canvasSize.x, canvasSize.y, &tempFilterDCArr);

		//lastSavedDC = txCreateDIBSection(canvasSize.x, canvasSize.y, &canvasArr);


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
	void drawLay ();
    void cleanOutputLay();
	void controlFilter();

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
	virtual void deleteButton () override;
};