#pragma once

#include "DrawBibliothek.h"
#include "WindowsLib.cpp"
#include "CLay.h"
#include "MainTools.h"
#include "ZoneSizeControl.h"

struct Canvas : Manager
{
    char name[MAX_PATH] = {};
	bool clearBackground = true;
    Vector canvasCoordinats = {};

    const int LayersNum = 100;
	int currentLayersLength = 0;
	int activeLayNum = 0; 
    Vector laysSize = {};
    HDC finalLay = NULL;
    COLORREF backgroungColor = TX_WHITE;
    CLay* lay = new CLay[LayersNum]{};
    ToolLay* toolLays = new ToolLay[LayersNum]{};
    int currentToolLength = 0;
    bool editingMode = false;
    bool activeTool = false;
    int DrawingModeLastTime = 0;

    ProgrammeDate currentDate;

    ZoneSizeControl zoneSizeControl;
    bool needFrameToWork = true;

    double scale = 1;
    double deltaScale = 0.1;
    int lastTimeButtonClicked = 0;
    int deltaBetween2Clicks = 300;
    Vector deltaPos = {};

    Canvas(AbstractAppData* _app, Rect _rect, const char* _name, HDC _closeDC = NULL);

	void controlSize();
	void controlSizeSliders ();
    void controlStretching();

	void saveHistory ();
	HDC playHistoryDC (int stepBack);
	void returnHistory(int stepsBack);
	void deleteHistory ();

    void drawCadre();

	void createLay ();
	bool controlLay ();
    void controlEditLay();
	void drawLays();
    void cleanOutputLay();

    void copyFinalLayOnFinalDC();

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
    virtual double& getScale();

    int getActiveLayNum();
    ToolLay* getNewToolLay();
    bool isDrawingModeChanged();
    Tool* getActiveTool();
    void setActiveToolLayNum(int num);
    int getLastNotStartedToolNum();

    virtual Vector getMousePos() override;

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
	virtual void deleteButton () override;
};