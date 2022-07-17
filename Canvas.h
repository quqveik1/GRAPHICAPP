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
    double scaleLastTime = 0;
    Vector outputDCSize = {};
    Vector posLastTime = {};
    double deltaScale = 0.1;
    int lastTimeButtonClicked = 0;
    int deltaBetween2Clicks = 300;
    Vector deltaPos = {};

    Canvas(AbstractAppData* _app, Rect _rect, const char* _name);

	void controlSize();
    void controlStretching();
    virtual Vector setNewCanvasSize(Vector newSize);

    void drawCadre();

	void createLay ();
	bool controlLay ();
    void controlEditLay();
	void drawLays();
    void cleanOutputLay();

    void copyFinalLayOnFinalDC();

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

    virtual void stretchCanvas(double percantageFromOriginal);

    int getActiveLayNum();
    ToolLay* getNewToolLay();
    bool isDrawingModeChanged();
    Tool* getActiveTool();
    void setActiveToolLayNum(int num);
    int getLastNotStartedToolNum();

    virtual Vector getMousePos() override;

    virtual void MoveWindow(Vector delta) override;
    virtual void MoveWindowTo(Vector pos) override;

    virtual void print(HDC _dc) override;
    virtual void resize(Vector newSize) override;

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
	virtual void deleteButton () override;
};