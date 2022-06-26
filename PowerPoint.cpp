#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "DrawBibliothek.cpp"
#include "GlobalOptions.h"
#include "Q_Buttons.h"
#include <cmath>
//#include "..\Brightness\Q_Filter.h"
#include "StandartFunctions.h"
#include "CurvesFilter.h"
#include "Slider.cpp"
#include "Canvas.h"
#include "TransferStructure.h"
#include "DLLFiltersManager.cpp"
#include "Tool.h"
#include "DLLToolsManager.cpp"
#include "CLay.cpp"
#include "MainTools.cpp"
#include "CanvasManager.h"
#include "ProgressBar.h"
#include "LoadManager.cpp"
#include "Lay.cpp"
#include "GlobalOptions.cpp"
#include "AppApi.cpp"


CSystemSettings SystemSettings;
CLoadManager LoadManager (&SystemSettings);






HDC TestPhoto;

//void bitBlt(RGBQUAD* dest, int x, int y, int sizeX, int sizeY, RGBQUAD* source, int originalSizeX = SystemSettings.DCMAXSIZE, int originalSizeY = SystemSettings.DCMAXSIZE, int sourceSizeX = SystemSettings.DCMAXSIZE, int sourceSizeY = SystemSettings.DCMAXSIZE);

void txSelectFontDC(const char* text, int sizey, HDC& dc, int sizex = -1);
void copyAndDelete (HDC dest, HDC source);
void invertDC (RGBQUAD* buf, unsigned int totalsize);


 



















CToolManager ToolManager;


struct Menu : Manager
{
    int lastSelected = 0;
    int currentSize = 0;
    Menu(AbstractAppData* _app, Rect _rect, Rect _handle, int _length = SystemSettings.ONEMENUBUTTONSNUM, bool _isDefaultActive = false) :
        Manager(_app, _rect, _length, _isDefaultActive, NULL, _handle, _app->systemSettings->MenuColor, true)
    {}

    virtual void drawOneLine(int lineNum) = NULL;
    virtual int onClickLine(Vector mp) = NULL;
    virtual void onUpdate() {};

    virtual void draw() override;
    virtual void onClick(Vector mp) override;


};




struct ToolsPalette : Menu
{
    
	ToolsPalette (AbstractAppData* _app, Rect _rect, int _length) :
		Menu(_app, _rect, { .pos = {0, 0}, .finishPos = {(double)_app->systemSettings->DCMAXSIZE, (double)_app->systemSettings->HANDLEHEIGHT}}, _length, true)
	{
        Window** tools = new Window*[ToolManager.currentLength];
        for (int i = 0; i < ToolManager.currentLength; i++)
        {
            tools[i] = new Window(app);
        }

        for (int i = 0; i < ToolManager.currentLength; i++)
        {
            tools[i]->rect = {.pos = {0, (double) i * 50}, .finishPos = {50, (double) (i + 1) * 50}};
            tools[i]->dc = ToolManager.tools[i]->getDC();
            tools[i]->finalDC = app->createDIBSection (tools[i]->getSize().x, tools[i]->getSize().y);
            tools[i]->originalRect = tools[i]->rect;
            addWindow (tools[i]);
            currentSize++;
        }
        
	}

    virtual void drawOneLine(int lineNum);
    virtual int  onClickLine(Vector mp);
};




//����, ������� ����� ��������� ��������� ��� ������������� �� ���� �������������
struct ToolMenu : Menu
{
    struct CanvasManager* canvasManager = NULL;
    HDC emptyToolDC = NULL;

    ToolMenu(AbstractAppData* _app, CanvasManager* _canvasManager, CLoadManager* _loadManager) :
        Menu(_app, { .pos = {_app->systemSettings->SizeOfScreen.x - 300, 300}, .finishPos = {_app->systemSettings->SizeOfScreen.x, _app->systemSettings->ONELAYTOOLSLIMIT * _app->systemSettings->BUTTONHEIGHT} }, { .pos = {0, 0}, .finishPos = {_app->systemSettings->DCVECTORSIZE.x, _app->systemSettings->HANDLEHEIGHT} }, _app->systemSettings->ONELAYTOOLSLIMIT, true),
        canvasManager(_canvasManager)
    {
        loadManager = _loadManager;
        emptyToolDC = loadManager->loadImage("addToolButton2.bmp");

        app->setColor(color, finalDC);
        app->rectangle(0, 0, rect.finishPos.x, rect.finishPos.y, finalDC);
        font = SystemSettings.MainFont * 1.5;
        app->selectFont(SystemSettings.FONTNAME, font, finalDC);
    }


    virtual void onUpdate();
    virtual void drawOneLine(int lineNum);
    virtual int  onClickLine(Vector mp);

};
		



struct TimeButton : Window
{
	int font;
	TimeButton (AbstractAppData* _app, Rect _rect, COLORREF _color = TX_RED, int _font = SystemSettings.MainFont)	:
		Window (_app, _rect, _color),
		font (_font)
	{}

	virtual void draw () override;
};

struct CloseButton : Window
{ 

	CloseButton (AbstractAppData* _app, Rect _rect, COLORREF _color, HDC _dc) :
		Window (_app, _rect, _color, _dc)
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

struct ResizeButton : Window
{
    Vector sizeLastTime;
    HDC nowIsFullScreen;
    HDC nowIsNotFullScreen;
    

    ResizeButton(AbstractAppData* _app, Rect _rect) :
        Window(_app, _rect),
        sizeLastTime(app->systemSettings->FullSizeOfScreen),
        nowIsFullScreen(app->loadManager->loadImage("FullScreenButton.bmp")),
        nowIsNotFullScreen (app->loadManager->loadImage("ResizeScreenButton.bmp"))
    {
    }

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};

struct ColorButton : Window
{
	ColorButton(AbstractAppData* _app, Rect _rect, COLORREF _color, HDC _dc) :
		Window (_app, _rect, _color, _dc)
	{
		draw ();
		reDraw = false;
	}

	virtual void onClick (Vector mp) override;
};

/*
struct RGBSliders : Manager
{
    Slider redColor;
    Slider greenColor;
    Slider blueColor;

    RGBSliders(AbstractAppData* _app, Rect _rect) :
        Manager(_app, _rect, 3),
        //redColor ()
    //{}

};
*/
																																					    
struct SizeButton : Window
{
	double *num;
	int sizeType;

	SizeButton(AbstractAppData* _app, Rect _rect, double *_num, int _sizeType) :
		Window (_app, _rect),
		num (_num), 
		sizeType (_sizeType)
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;

};

struct CleanButton : Window
{
	Canvas *mainCanvas;

	CleanButton (AbstractAppData* _app, Rect _rect, COLORREF _color, Canvas *_mainCanvas, HDC _dc = NULL, const char *_text = "") :
		Window (_app, _rect, _color, _dc),
		mainCanvas (_mainCanvas)
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

struct LaysMenu : Manager
{
	CanvasManager* canvasManager;
	int sectionHeight;
    int sectionFont;
    HDC addNewLayButton;
    Vector buttonSize;

    LaysMenu(AbstractAppData* _app, Rect _rect, CanvasManager* _canvasManager) :
        Manager(_app, _rect, 0, true, NULL, { .pos = {0, 0}, .finishPos = {_rect.getSize().x, SystemSettings.HANDLEHEIGHT} }),
        canvasManager(_canvasManager),
        sectionHeight(systemSettings->HANDLEHEIGHT),
        sectionFont(systemSettings->MainFont * 0.9),
        addNewLayButton(LoadManager.loadImage("AddNewCanvas2.bmp")),
        buttonSize ({ SystemSettings.BUTTONWIDTH, SystemSettings.HANDLEHEIGHT })
	{
		rect.finishPos.y = rect.pos.y + handle.rect.getSize().y;
		handle.rect.finishPos.x = rect.getSize().x;
		handle.color = color;
		handle.text = "����";
        handle.font = SystemSettings.MainFont;
	}

	virtual void draw() override;
	virtual void onClick(Vector mp) override;
};


struct OpenManager : Window
{
	Manager *openManager;
    bool isOpeningAnotherList = false;

	OpenManager (AbstractAppData* _app, Rect _rect, COLORREF _color, Manager *_manager, HDC _dc = NULL, const char *_text = "")	:
		Window (_app, _rect, _color, _dc, NULL, _text),
		openManager (_manager)
	{
        format = DT_VCENTER;
    }

    OpenManager (AbstractAppData* _app)	:
        Window (_app),
		openManager (NULL)
	{
        format = DT_VCENTER;
    }

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

struct StringButton : Window
{
	bool advancedMode;
	char *inText;
	int cursorPosition;
	int textLen;
	int lastButton;
	int lastTimeClicked;
	bool onlyNums;
	const int MaxNum;



	StringButton(AbstractAppData* _app, Rect _rect, COLORREF _color, char *_redactingText, int _redactingTextLength, Manager *_manager, int _MaxNum = 20, bool _onlyNums = false) :
		Window(_app, _rect, _color, NULL, _manager),
		inText (_redactingText),
		advancedMode(true),
		cursorPosition(_redactingTextLength - 1),
		textLen (_redactingTextLength + 1),
		lastButton (0),
		lastTimeClicked (0),
		onlyNums (_onlyNums),
		MaxNum (_MaxNum)
	{}


	virtual void draw () override;
	void checkSymbols ();
	void cursorMovement (int side);
	void backSpace ();
};


struct NumChange : Manager
{
	StringButton stringButton;
	SizeButton plusNum;
	SizeButton minusNum;
	HDC plusMinusDC;
	Slider slider;
	double *num;
	const int maxNum;
	const int minNum;
	char inText[32] = {};

	NumChange (AbstractAppData* _app, Rect _mainRect, Rect _stringRect, Rect _plusRect, Rect _minusRect, Rect _sliderRect, HDC _plusMinusDC, double _sliderQuadrateScale, int _numLength, double *_num) :
		Manager (_app, _mainRect, 4),
		num (_num),
		stringButton (_app, _stringRect, TX_WHITE, inText, _numLength, this, 20, true),
		plusNum (_app, _plusRect, _num, +1),
		minusNum (_app, _minusRect, _num, -1),
		plusMinusDC (_plusMinusDC),
		minNum (1),
		maxNum (20),
		slider (_app, _sliderRect, _num, _sliderQuadrateScale, 1, 20)
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};
 
struct StatusBar : Manager
{

	TimeButton *timeButton = NULL;
	ProgressBar *progressBar = NULL;

	StatusBar(AbstractAppData* _app, Rect _rect, COLORREF _color) :
		Manager(_app, _rect, 3, true, NULL, {}, _color)
	{}


	virtual void draw() override;
};

struct TouchButton : Window
{
    bool *flag = NULL;

    TouchButton (AbstractAppData* _app, Rect _rect, HDC _dc, bool *_flag = NULL) :
        Window (_app, _rect, SystemSettings.MenuColor, _dc),
        flag (_flag)
    {
    }

    virtual void onClick (Vector mp) override;
};

struct AddCanvasButton : TouchButton
{
    CanvasManager* canvasManager;
    AddCanvasButton(AbstractAppData* _app, Rect _rect, HDC _dc, CanvasManager* _canvasManager) :
        TouchButton (_app, _rect, _dc),
        canvasManager (_canvasManager)
    {
    }

    virtual void onClick(Vector mp) override;
};


struct List : Manager
{
    int itemHeight;
    OpenManager** items;
    Vector oneItemSize;
    bool *isThisItemList;
    int lastClickedItemNum = -1;
    bool mayFewWindowsBeOpenedAtTheSameTime;
    int activeItemCircleSize = 3;

    List (AbstractAppData* _app, Vector _pos, Vector _oneItemSize, int _maxLength, bool _mayFewWindowsBeOpenedAtTheSameTime = true) :
        Manager (_app, {.pos = _pos, .finishPos = {_pos.x + _oneItemSize.x, _pos.y + _maxLength * _oneItemSize.y }}, _maxLength, false),
        mayFewWindowsBeOpenedAtTheSameTime (_mayFewWindowsBeOpenedAtTheSameTime),
        oneItemSize (_oneItemSize),
        itemHeight (systemSettings->HANDLEHEIGHT)
    {
        items = new OpenManager*[length];
        for (int i = 0; i < length; i++)  items[i] = new OpenManager (app);

        isThisItemList = new bool[length]{};
    }

    void addNewItem (Window *openButton, HDC dc = NULL, const char *text = NULL);
    Vector getNewSubItemCoordinats ();
    List* addSubList (const char *ListText, int length = NULL);
    void controlRect();

    virtual void draw() override;
	virtual void onClick(Vector mp) override;
};  

struct SaveImages : Window
{
    CanvasManager* canvasManager;

    SaveImages(AbstractAppData* _app, CanvasManager* _canvasManager) :
        Window(_app, {}, SystemSettings.MenuColor, NULL, NULL, "", false),
        canvasManager (_canvasManager)
    {
    }

    virtual void draw() override;
};

struct Handle : Manager
{
    int addToBackElemetsPos[10] = {};
    int numberOfAddToBackElements = 0;   
    bool wasCommonHandlePlaceClicked = false;
    Vector lastTimeMousePos = {};

    Handle(AbstractAppData* _app, Rect _rect) :
        Manager(_app, _rect, 10, true, NULL, {}, _app->systemSettings->MenuColor)
    {}

    virtual bool addWindowToStart(Window* window);
    virtual bool addWindowToBack(Window* window);

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};




bool IsRunning = true;
int Radius = 2;

void Engine (Manager *manager);

void RECTangle (const Rect rect, HDC dc = txDC ());
void shiftArrBack    (char arr[], int length);
void shiftArrForward (char arr[], int length);
bool checkDeltaTime (int lastTimeClicked);
void printfDCS (const char *str = "");
bool swapDC(HDC dest, int xDest, int yDest, int wDest, int hDest,
    HDC src, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rOp);




int main (int argc, int *argv[])
{
    PowerPoint* appData = new PowerPoint;
    appData->systemSettings = &SystemSettings;
    appData->currColor = &(SystemSettings.DrawColor);
    appData->loadManager = &LoadManager;
    

    setDefaultSystemSettings(appData->systemSettings);
    
    appData->systemSettings->read("Settings\\FullSettings.settings");
    setSystemSettings(appData->systemSettings, "Settings\\Settings.txt");  
    setDynamicSystemSettings(appData->systemSettings);


    _txWindowStyle = appData->systemSettings->WindowStyle;
    _txSwapBuffers = swapDC;
    appData->systemSettings->MAINWINDOW = txCreateWindow (appData->systemSettings->FullSizeOfScreen.x, appData->systemSettings->FullSizeOfScreen.y);

    appData->changeWindow(appData->systemSettings->SizeOfScreen);
    appData->setResized(false);

    Manager* manager = new Manager(appData, { .pos = {0, 0}, .finishPos = appData->systemSettings->FullSizeOfScreen }, 20, true, NULL, {}, TX_RED);

    ToolSave toolSave = {};

	HDC addNewCanvasDC = {};
	HDC oldDC = LoadManager.loadImage ("addNewCanvas.bmp");
	compressImage (appData, addNewCanvasDC, {50, 50}, oldDC, {225, 225});
	appData->deleteDC(oldDC);

    /*
	StatusBar* statusBar = new StatusBar(appData, {.pos = {0, appData->systemSettings->SizeOfScreen.y - 20}, .finishPos = {appData->systemSettings->SizeOfScreen.x,  appData->systemSettings->SizeOfScreen.y}} , TX_BLUE);
		statusBar->progressBar =  new ProgressBar (appData, {.pos = {0, 0}, .finishPos = statusBar->rect.getSize()}, TX_GREEN);
		statusBar->progressBar->manager = statusBar;
		statusBar->timeButton = new TimeButton(appData, {.pos = {statusBar->rect.getSize().x - 65, 0}, .finishPos = statusBar->rect.getSize()}, TX_WHITE);
		statusBar->timeButton->manager = statusBar;
      */
    

    CanvasManager* canvasManager = new CanvasManager(appData, { .pos = {0, 0}, .finishPos = appData->systemSettings->FullSizeOfScreen }, addNewCanvasDC);
    appData->canvasManager = canvasManager;
	manager->addWindow (canvasManager);

    //manager->addWindow(statusBar);

    if (appData->systemSettings->debugMode >= 0) printf("����������� ������ �����������\n");
    DLLToolsManager* dllToolsManager = new DLLToolsManager(appData, "Settings\\DLLPathList.txt");
    dllToolsManager->loadLibs();
    dllToolsManager->addToManager(&ToolManager);
    if (appData->systemSettings->debugMode >= 0) printf("����������� �����������\n");
    appData->toolManager = dllToolsManager;
    

	ToolsPalette *toolsPallette = new ToolsPalette(appData, {.pos = {0, 100}, .finishPos = {50, (double)ToolManager.currentLength * 50 + appData->systemSettings->HANDLEHEIGHT + 100}}, ToolManager.currentLength);
    manager->addWindow(toolsPallette);

    ToolMenu* toolMenu = new ToolMenu(appData, canvasManager, &LoadManager);
    manager->addWindow(toolMenu);   


	Manager *menu = new Manager(appData, {.pos = {appData->systemSettings->SizeOfScreen.x - 712, 300}, .finishPos = {appData->systemSettings->SizeOfScreen.x - 300, 600}}, 3, false, LoadManager.loadImage ("ColorsMenu.bmp"), {.pos = {0, 0}, .finishPos = {412, 50}});
	manager->addWindow (menu);
	
	Manager *colorManager = new Manager(appData, {.pos = {10, 180}, .finishPos = {170, 220}}, 3, true, NULL, {}, RGB (26, 29, 29));
	    menu->addWindow (colorManager);

			ColorButton *redColor = new ColorButton(appData, {.pos = {0, 0}, .finishPos = {40, 40}}, RGB (255, 0, 0), LoadManager.loadImage ("RedButton.bmp"));
			colorManager->addWindow(redColor);
	
			ColorButton *blueColor = new ColorButton(appData, {.pos = {60, 0}, .finishPos = {100, 40}}, RGB (0, 0, 255), LoadManager.loadImage ("BlueButton.bmp"));
			colorManager->addWindow(blueColor);

			ColorButton *greenColor = new ColorButton(appData, {.pos = {120, 0}, .finishPos = {160, 40}}, RGB (0, 255, 0), LoadManager.loadImage ("GreenButton.bmp"));
			colorManager->addWindow(greenColor);

	OpenManager *openManager = new OpenManager(appData, {.pos = {15, 135}, .finishPos = {36, 153}}, TX_WHITE, colorManager, LoadManager.loadImage ("OpenColorButton.bmp"));
	menu->addWindow (openManager);



    DLLFiltersManager* dllManager = new DLLFiltersManager(appData, "Settings\\DLLPathList.txt");
    dllManager->loadLibs ();
    dllManager->addToManager(manager);
    if (SystemSettings.debugMode >= 0) printf("������� �����������\n");

    LaysMenu* laysMenu = new LaysMenu (appData, {.pos = {0, 700}, .finishPos = {appData->systemSettings->BUTTONWIDTH, 1000}}, canvasManager);
    manager->addWindow(laysMenu);

    //Curves *curves = new Curves ({.pos = {500, 500}, .finishPos = {500 + 443, 500 + 360}}, LoadManager.loadImage("Brightness.bmp"));
    //manager->addWindow(curves);

	Handle* mainhandle = new Handle(appData, { .pos = {0, 0}, .finishPos = {appData->systemSettings->SizeOfScreen.x, appData->systemSettings->HANDLEHEIGHT} });
    manager->addWindow(mainhandle);

		CloseButton* closeButton = new CloseButton(appData, { .pos = {appData->systemSettings->SizeOfScreen.x - appData->systemSettings->BUTTONWIDTH, 0}, .finishPos = {appData->systemSettings->SizeOfScreen.x, appData->systemSettings->HANDLEHEIGHT} }, TX_RED, LoadManager.loadImage("CloseButton4.bmp"));
		mainhandle->addWindowToBack(closeButton);

        ResizeButton* resizeButton = new ResizeButton(appData, { .pos = {appData->systemSettings->SizeOfScreen.x - appData->systemSettings->BUTTONWIDTH * 2, 0}, .finishPos = {appData->systemSettings->SizeOfScreen.x - appData->systemSettings->BUTTONWIDTH, appData->systemSettings->HANDLEHEIGHT} });
        mainhandle->addWindowToBack(resizeButton);

        AddCanvasButton* addNewCanvas = new AddCanvasButton(appData, {.pos = {0, 0}, .finishPos = {appData->systemSettings->BUTTONWIDTH, appData->systemSettings->HANDLEHEIGHT}}, LoadManager.loadImage ("AddNewCanvas2.bmp"), canvasManager);
		mainhandle->addWindowToStart(addNewCanvas);

        List* openWindows = new List (appData, { appData->systemSettings->BUTTONWIDTH, appData->systemSettings->HANDLEHEIGHT}, { appData->systemSettings->BUTTONWIDTH * 5, appData->systemSettings->HANDLEHEIGHT}, 5);
        manager->addWindow(openWindows);
        OpenManager* openWindowsManager = new OpenManager (appData, {.pos = {appData->systemSettings->BUTTONWIDTH, 0}, .finishPos = {appData->systemSettings->BUTTONWIDTH * 2, appData->systemSettings->HANDLEHEIGHT}}, TX_WHITE, openWindows, LoadManager.loadImage ("OpenWindows.bmp"));
        mainhandle->addWindowToStart(openWindowsManager);
        
        openWindows->addNewItem (menu, NULL, "����");
        openWindows->addNewItem (toolsPallette, NULL, "�����������");
        openWindows->addNewItem (laysMenu, NULL, "����");
        openWindows->addNewItem (toolMenu, NULL, "���������������� ����");
        List* filters = openWindows->addSubList("�������");
        manager->addWindow (filters);
            //filters->addNewItem (curves, NULL, "������");
            for (int i = 0; i < dllManager->currLoadWindowNum; i++)
            {
                filters->addNewItem(dllManager->dllWindows[i], NULL, dllManager->dllWindows[i]->name);
            }


        List* systemList = new List(appData, { appData->systemSettings->BUTTONWIDTH * 2, appData->systemSettings->HANDLEHEIGHT }, { appData->systemSettings->BUTTONWIDTH * 5, appData->systemSettings->HANDLEHEIGHT }, 1);
        manager->addWindow(systemList);
        OpenManager* openSystemList = new OpenManager(appData, { .pos = {appData->systemSettings->BUTTONWIDTH * 2, 0}, .finishPos = {appData->systemSettings->BUTTONWIDTH * 3, appData->systemSettings->HANDLEHEIGHT} }, TX_WHITE, systemList, LoadManager.loadImage("SettingsIcon.bmp"));
        mainhandle->addWindowToStart(openSystemList);


        SaveImages* saveImages = new SaveImages(appData, canvasManager);
        systemList->addNewItem(saveImages, NULL, "��������� �����������");
            
	

    if (SystemSettings.debugMode == 6) _getch();



	txBegin ();
	Engine (manager);
    saveSystemSettings(appData->systemSettings, "Settings\\Settings.txt");
    appData->systemSettings->save("Settings\\FullSettings.settings");
	txEnd ();
    txDisableAutoPause();
    //delete manager;
    //LoadManager.deleteAllImages ();
	return 0;
}
                                                                                            

LRESULT CALLBACK CtrlWindowFunc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_SIZING)
    {
        
    }
    return 0;
}





void List::draw()
{

    controlRect();
    standartManagerDraw (this);

    for (int i = 0; i < newButtonNum; i++)
    {
        //pointers[i]->advancedMode = advancedMode;
        app->setColor (SystemSettings.SecondMenuColor, finalDC, SystemSettings.SIDETHICKNESS);
        app->line (0, i * itemHeight, rect.getSize().x, i * itemHeight, finalDC);

        if (items[i]->openManager->advancedMode) 
            app->ellipse (rect.getSize().x * 0.9 - activeItemCircleSize, ((double)i + 0.5) * itemHeight - activeItemCircleSize, rect.getSize().x * 0.9 + activeItemCircleSize, ((double)i + 0.5) * itemHeight + activeItemCircleSize, finalDC);

        if (isThisItemList[i] && !advancedMode)
        {
            items[i]->openManager->advancedMode = false;
        }
    }

    if (!getMBCondition()) lastClickedItemNum = -1;
    setMbLastTime();
}

void List::onClick (Vector mp)
{
    mousePos = getMousePos();
    int clikedButtonNum = standartManagerOnClick (this, mp);
    if (clikedButtonNum >= 0 && clikedButtonNum != lastClickedItemNum && !isClickedLastTime())
    {
        //printf ("last: %d, current: %d\n", lastClickedItemNum, clikedButtonNum);
        if (pointers[clikedButtonNum]->advancedMode && mayFewWindowsBeOpenedAtTheSameTime)
        {
            clickButton(pointers[clikedButtonNum], this, mp);
        }
        lastClickedItemNum = clikedButtonNum;
    }
}


bool Handle::addWindowToStart(Window* window)
{
    Rect windowRect = {};
    windowRect.pos.x = rect.pos.x + app->systemSettings->BUTTONWIDTH * (getCurLen() - numberOfAddToBackElements);
    windowRect.pos.y = rect.pos.y;

    windowRect.finishPos.x = rect.pos.x + app->systemSettings->BUTTONWIDTH * (getCurLen() - numberOfAddToBackElements + 1);
    windowRect.finishPos.y = rect.finishPos.y;

    window->resize(windowRect);

    return addWindow(window);
}

bool Handle::addWindowToBack(Window* window)
{
    Rect windowRect = {};
    windowRect.pos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * (getCurLen() + 1);
    windowRect.pos.y = rect.pos.y;

    windowRect.finishPos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * getCurLen();
    windowRect.finishPos.y = rect.finishPos.y;

    window->resize(windowRect);

    addToBackElemetsPos[numberOfAddToBackElements] = getCurLen();
    numberOfAddToBackElements++;

    return addWindow(window);
}


void Handle::draw()
{
    standartManagerDraw(this);

    if (app->wasResized())
    {
        Rect newRect = { .pos = rect.pos, .finishPos = {app->systemSettings->SizeOfScreen.x, rect.pos.y + getSize().y} };
        resize(newRect);

        for (int i = 0; i < numberOfAddToBackElements; i++)
        {
            Rect windowRect = {};
            windowRect.pos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * (i + 1);
            windowRect.pos.y = rect.pos.y;

            windowRect.finishPos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * i;
            windowRect.finishPos.y = rect.finishPos.y;
            pointers[i]->resize(windowRect);
        }
    }

    if (wasCommonHandlePlaceClicked)
    {
        Vector superAbsMP = getAbsMousePos() + app->systemSettings->ScreenPos;
        Vector delta = superAbsMP - lastTimeMousePos;
        if (app->wasResized())
        {
            delta = { 0, 0 };
        }

        if (app->systemSettings->debugMode >= 2) printf("delta: {%lf, %lf}\n", delta.x, delta.y);
        if (app->systemSettings->debugMode >= 2) printf("mp: {%lf, %lf}\n", superAbsMP.x, superAbsMP.y);

        if (delta != 0)
        {
            app->systemSettings->ScreenPos += delta;
            app->changeWindow({}, app->systemSettings->ScreenPos);
        }

        lastTimeMousePos = superAbsMP;
        if (wasCommonHandlePlaceClicked && getMBCondition() == 0)wasCommonHandlePlaceClicked = 0;
    }

    setMbLastTime();
}

void Handle::onClick(Vector mp)
{
    int resultOfClicking = standartManagerOnClick(this, mp);

    if (resultOfClicking == -1 && !wasCommonHandlePlaceClicked)
    {
        if (app->isFullScreen())
        {
            app->changeWindow(app->systemSettings->lastTimeSizeOfScreen);
            app->setResized();
        }
        lastTimeMousePos = getAbsMousePos() + app->systemSettings->ScreenPos;
        wasCommonHandlePlaceClicked = true;
    }
}


void SaveImages::draw()
{
    advancedMode = false;

    //������� ������ ���� ����� �� ������
    if (!canvasManager->getActiveCanvas())
    {
        txMessageBox("������ ���������, ����� �� ������", "������");
        return;
    }

    const char* pathToSave = getCustomFilePathForSaving("����� ���������� ��������", "Image (*.bmp)", "bmp");
    char fullPath[MAX_PATH] = {};
    strcpy(fullPath, pathToSave);

    if (fullPath[0] == NULL)
    {
        return;
    }

    HDC saveDC = NULL;

    saveDC = canvasManager->getActiveCanvas()->getImageForSaving();


    int result = app->saveImage(saveDC, fullPath);
    app->deleteDC(saveDC);
    

    if (!result) txMessageBox("������ �� �����������", "������");
}

void StatusBar::draw()
{
	app->setColor(color, finalDC);
	app->rectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);

	if (timeButton)timeButton->draw();
	
	progressBar->draw();

	int currentNum = NULL;
	if (progressBar->currentNum)
	{
		currentNum = *progressBar->currentNum;
	}

	if (currentNum)app->bitBlt(finalDC, 0, 0, rect.getSize().x, rect.getSize().y, progressBar->finalDC);

	char text[32] = {};
	app->setColor(TX_WHITE, finalDC);
	app->selectFont("Arial", SystemSettings.MainFont, finalDC);
	if (currentNum != 0)
	{
		sprintf(text, "������: ����������� ������");
	}

	if (currentNum == 0)
	{
		sprintf(text, "������: ������� ����� ���");
	}
	app->setAlign(TA_LEFT, finalDC);
	app->drawText(10, 0, getSize().x, getSize().y, text, finalDC, DT_LEFT | DT_VCENTER);


	if (timeButton) app->bitBlt (finalDC, timeButton->rect.pos.x, timeButton->rect.pos.y, timeButton->getSize().x, timeButton->getSize().y, timeButton->finalDC);

}

void List::addNewItem (Window *openButton, HDC dc/* = NULL*/, const char *text/* = NULL*/)
{
    Rect newRect = {.pos = {0, (double)(newButtonNum) * itemHeight}, .finishPos = {rect.getSize().x, (double)(newButtonNum + 1) * itemHeight}};
    items[newButtonNum]->rect =  newRect;
    items[newButtonNum]->color =  SystemSettings.MenuColor;
    items[newButtonNum]->openManager =  (Manager*)openButton;
    items[newButtonNum]->dc =  dc;
    items[newButtonNum]->text =  text;
    items[newButtonNum]->reInit ();


    addWindow (items[newButtonNum]);
}


void List::controlRect()
{
    rect.finishPos = { rect.pos.x + oneItemSize.x, rect.pos.y + oneItemSize.y * newButtonNum};
}


List* List::addSubList (const char *ListText, int newListLength/* = NULL*/)
{
    if (!newListLength) newListLength = length;
    List* subList = new List(app, getNewSubItemCoordinats(), oneItemSize, newListLength);

    isThisItemList[newButtonNum] = true;
    addNewItem (subList, NULL, ListText);

    return subList;
}

Vector List::getNewSubItemCoordinats ()
{
    return Vector {rect.finishPos.x, rect.pos.y + (double)(newButtonNum) * itemHeight}; 
}


void TouchButton::onClick (Vector mp)
{
    if (!isClickedLastTime()) *flag = true;
    setMbLastTime();
}

void AddCanvasButton::onClick (Vector mp)
{
    if (!isClickedLastTime()) canvasManager->addCanvas();
    setMbLastTime();
}

void txSelectFontDC(const char* text, int sizey, HDC &dc, int sizex/* = -1*/)
{
	txSelectFont(text, sizey, sizex, FW_DONTCARE, false, false, false, 0, dc);
}

void NumChange::draw ()
{
	if (! (*num == 0))
		sprintf (inText, "%d", (int) *num);

	stringButton.draw ();

	if (!strcmp ("", inText))
		*num = 0;

	sscanf  (inText, "%lf", num);

	//app->bitBlt (plusNum.rect.pos.x, plusNum.rect.pos.y, plusMinusDC); 

	//if (backGround) app->bitBlt (minusNum.rect.pos.x, minusNum.rect.pos.y, backGround);

	plusNum.draw  ();
	minusNum.draw (); 

	int numBeforeSlider = *num;
	slider.draw ();
	//checkTextLen (*num, numBeforeSlider, &stringButton.textLen, &stringButton.cursorPosition);

	//if (getMBCondition() != 1 && plusNum.isClickedLastTime()  == true) plusNum.isClickedLastTime() = false;
	//if (getMBCondition() != 1 && minusNum.isClickedLastTime() == true) minusNum.isClickedLastTime() = false;

	if (manager->activeWindow != this) activeWindow = NULL;
}


void NumChange::onClick (Vector mp)
{
	//int mx = txMouseX ();
	//int my = txMouseY ();

    /*
	 if (stringButton.rect.inRect (mx, my))
	 {
		 activeWindow = &stringButton;
	 }

	if (plusNum.rect.inRect (mx, my) && !plusNum.isClickedLastTime())
	{
		activeWindow = &plusNum;

		if (*num >= maxNum) return;
		if ((*num == 9) || (*num == 99) || (*num == 999) || (*num == 9999) && !plusNum.isClickedLastTime() && *num < maxNum) //fix?!
		{
			stringButton.textLen++;
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		if (!plusNum.isClickedLastTime())
		{
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		//plusNum.onClick ();
		plusNum.isClickedLastTime() = true;
	}

	if (minusNum.rect.inRect (mx, my) && !minusNum.isClickedLastTime())
	{
		activeWindow = &minusNum;

		if (*num <= minNum) return;

		if ((*num == 10) || (*num == 100) || (*num == 1000) || (*num == 10000)&& !plusNum.isClickedLastTime() && minNum < *num)
		{
			stringButton.textLen--;
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		if (!plusNum.isClickedLastTime())
		{
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		//minusNum.onClick ();
		minusNum.isClickedLastTime() = true;
	}


	if (slider.rect.inRect (mx, my))
	{
		activeWindow = &slider;

		bool changeTextLen = false;
		int numBeforeSlider = *num;

		//slider.onClick();

		//checkTextLen (*num, numBeforeSlider, &stringButton.textLen, &stringButton.cursorPosition);
	}
   */
}

void ToolsPalette::drawOneLine(int lineNum)
{
    pointers[lineNum]->draw();
    //app->drawOnScreen (pointers[lineNum]->dc);
    if (pointers[lineNum]->advancedMode) app->bitBlt(finalDC, pointers[lineNum]->rect.pos.x, pointers[lineNum]->rect.pos.y + handle.rect.finishPos.y, pointers[lineNum]->rect.finishPos.x, pointers[lineNum]->rect.finishPos.y, pointers[lineNum]->finalDC);

    if (app->systemSettings->DrawingMode - 1 == lineNum)
    {
        app->setColor(TX_WHITE, finalDC);
        app->rectangle(pointers[lineNum]->rect.pos.x, pointers[lineNum]->rect.pos.y + handle.rect.finishPos.y, pointers[lineNum]->rect.pos.x + pointers[lineNum]->rect.getSize().x * 0.1, pointers[lineNum]->rect.pos.y + pointers[lineNum]->rect.getSize().y * 0.1 + handle.rect.finishPos.y, finalDC);
    }

    app->setColor(TX_BLACK, finalDC);
    app->line(0, pointers[lineNum]->rect.pos.y + handle.rect.getSize().y, rect.getSize().x, pointers[lineNum]->rect.pos.y + handle.rect.getSize().y, finalDC);
}

int ToolsPalette::onClickLine(Vector mp)
{   
    for (int lineNum = currentSize - 1; lineNum >= 0; lineNum--)
    {

        bool missClicked = false;
        if (pointers[lineNum]->rect.inRect(mp.x, mp.y))
        {
            activeWindow = pointers[lineNum];
            clickButton(pointers[lineNum], this, mp);
            //pointers[lineNum]->onClick(mp - pointers[lineNum]->rect.pos);
            //pointers[lineNum]->isClickedLastTime() = true;
            SystemSettings.DrawingMode = lineNum + 1;
            lastSelected = lineNum;

            missClicked = false;

            if (pointers[lineNum]->advancedMode) return missClicked + 2;
        }
        else
        {
            missClicked = true;
        }

        
    }
    return  1;
}

void ToolMenu::onUpdate()
{   
    Canvas* activeCanvas = canvasManager->getActiveCanvas();

    if (activeCanvas) currentSize = activeCanvas->getCurrentToolLengthOnActiveLay() + 1;
    rect.finishPos.y = currentSize * SystemSettings.BUTTONHEIGHT + handle.rect.finishPos.y + rect.pos.y;
}


void ToolMenu::drawOneLine(int lineNum)
{
    if (!canvasManager->getActiveCanvas()) return;

    const char* nameOfTool = NULL;
    HDC toolDC = NULL;
    CLay* lay = canvasManager->getActiveCanvas()->getActiveLay();

    int notStartedNum = canvasManager->getActiveCanvas()->getLastNotStartedToolNum();
    if (lineNum == notStartedNum)
    {
        nameOfTool = "����� ����������";
        toolDC = emptyToolDC;
    }
    else
    {
        Tool* tool = lay->getToolLays()[lineNum]->getTool();

        assert(tool);

        toolDC = tool->getDC();
        nameOfTool = tool->getName();
    }

    lastSelected = lay->getActiveToolLayNum();


    char outputText[MAX_PATH] = {};
    sprintf(outputText, "%d - %s", lineNum + 1, nameOfTool);

    int linePosY = SystemSettings.BUTTONHEIGHT * lineNum + handle.rect.finishPos.y;

    app->bitBlt(finalDC, 0, linePosY, SystemSettings.BUTTONWIDTH, linePosY + SystemSettings.BUTTONHEIGHT, toolDC);
    app->setColor(SystemSettings.MenuColor, finalDC);
    app->rectangle(SystemSettings.BUTTONWIDTH, linePosY, rect.getSize().x, linePosY + SystemSettings.BUTTONHEIGHT,                         finalDC);

    app->setColor(SystemSettings.TextColor, finalDC);
    app->drawText (SystemSettings.BUTTONWIDTH, linePosY, rect.getSize().x, linePosY + SystemSettings.BUTTONHEIGHT, outputText, finalDC, SystemSettings.TEXTFORMAT);

    app->setColor(TX_BLACK, finalDC);
    app->line(0, linePosY, rect.getSize().x, linePosY, finalDC);

    if (lastSelected == lineNum)
    {
        app->setColor(TX_WHITE, finalDC);
        app->rectangle(0, linePosY, 5, linePosY + 5, finalDC);
    }
}


int ToolMenu::onClickLine(Vector mp)
{
    double my = mp.y;
    //int my = mp.y;

    int buttonNum = floor (my / SystemSettings.BUTTONWIDTH);


    if (!(buttonNum >= 0 && buttonNum <= currentSize))  return 0;

    canvasManager->getActiveCanvas()->setActiveToolLayNum(buttonNum);

    return 1;
}




void StringButton::draw ()
{
	$s;
	bool switched = false;
	bool isShifted = false;
	bool deleteChar = false;


	if (manager->getActiveWindow () == this)
	{
		if (checkDeltaTime (lastTimeClicked))
		{
			if (app->getAsyncKeyState(VK_LEFT) && cursorPosition >= 0)
			{
				cursorMovement (VK_LEFT);
				//printf ("left");
				switched = true;
			}
			if (app->getAsyncKeyState(VK_RIGHT) && cursorPosition <= textLen - 2)
			{
				cursorMovement (VK_RIGHT);
				switched = true;
			}

			if (app->getAsyncKeyState (VK_BACK))
			{
				backSpace ();	
			}
		
			checkSymbols ();
		}
	
		if (clock() % 500 < 250)
		{
			if (!switched)
				shiftArrForward (&inText[cursorPosition + 1], 10);

			inText[cursorPosition + 1] = '|';
			printf ("");
		}
		else
		{
			if (!switched)
				shiftArrForward (&inText[cursorPosition + 1], 10);

			inText[cursorPosition + 1] = ' ';
		}
	}

	//txSetTextAlign (TA_LEFT);
	//app->setColor (TX_WHITE);
	
	//txTextOut (rect.pos.x, rect.pos.y, inText);

	if (manager->getActiveWindow () == this) shiftArrBack (&inText[cursorPosition + 1], 10);

	if (onlyNums)
	{
		double num;
		if (!strcmp ("", inText)) num = 0;

		sscanf  (inText, "%lf", &num);
		if (num > MaxNum) 
		{
			if ( ((int) log10 (num)) - ((int) log10 (MaxNum)) >= 1) 
			{
				cursorPosition--;
			}
			sprintf (inText, "%d", (int) MaxNum);
		}
	}

	return;
}

void stringToInt (const char *str, const int  strLen, int &num)
{
	num = 0;
	for (int i = 0; i < strLen; i++)
	{
		num += (str[i] - 48) * pow (10, strLen - i - 1);
	}
}


void StringButton::checkSymbols ()
{
	if (!_kbhit ())	return;
	int symbol = _getch ();

	if (! (symbol >= 48 && symbol <= 57) && onlyNums) return;
	if (lastButton == symbol || symbol == '\b') return;
	if (symbol == 48 &&	onlyNums && cursorPosition == -1) return;

	lastTimeClicked = clock();
	shiftArrForward(&inText[cursorPosition + 1], textLen - cursorPosition);
	inText[cursorPosition + 1] = symbol;
	cursorPosition++;
	textLen++;
}

bool checkDeltaTime (int lastTimeClicked)
{
	return clock () - lastTimeClicked > SystemSettings.DELTACLOCKTIME;
}

void StringButton::backSpace ()
{
	if (cursorPosition >= 0)
	{
		lastTimeClicked = clock();
		shiftArrBack(&inText[cursorPosition], textLen - cursorPosition  + 1);
		cursorPosition--;
		textLen--;
	}
}


void StringButton::cursorMovement (int side)
{
	bool validMovement = false;	
	if (side == VK_RIGHT) validMovement = (cursorPosition < textLen - 2);
	if (side == VK_LEFT)  validMovement = (cursorPosition >= 0);

	if (validMovement && clock () - lastTimeClicked > SystemSettings.DELTACLOCKTIME)
	{
		lastTimeClicked = clock();
		//shiftArrBack(&inText[cursorPosition + 1], textLen - cursorPosition);
		if (side == VK_RIGHT) cursorPosition++;
		if (side == VK_LEFT) cursorPosition--;
		shiftArrForward(&inText[cursorPosition + 1], textLen - cursorPosition);
	}
}

void shiftArrForward (char arr[], int length)
{
    assert (arr);
	char copyChar = arr[0];

	for (int i = 1; i < length; i++)
	{
		char rememberArr = arr[i];
		arr[i] = copyChar;
		copyChar = rememberArr;
	}
}

void shiftArrBack (char arr[], int length)
{

    assert (arr);
	char copyChar = '\0';

	for (int i = 0; i < length; i++)
	{
		if (i == length - 1)
		{
			arr[i] = '\0';
		}
		else
		{
			arr[i] = arr[i + 1];
		}
		
	}
}


void OpenManager::onClick (Vector mp)
{
    if (!isClickedLastTime())
    {
        openManager->advancedMode = !openManager->advancedMode;
        openManager->draw();
    }   
    setMbLastTime();
}

void OpenManager::draw()
{
    standartDraw (this);
	app->setColor (SystemSettings.TextColor, finalDC, SystemSettings.MainFont); 
    setMbLastTime();
}

void Engine (Manager *manager)
{
    assert (manager); 
    AbstractAppData* app = manager->app;
    assert(app);

    HDC outputDC = app->createDIBSection(app->systemSettings->SizeOfScreen.x, app->systemSettings->SizeOfScreen.y);
    txDC() = outputDC;

    bool wasResizedInLastFrame = false;

	for (;;)
	{
        txClearConsole();
        if (SystemSettings.debugMode == -1 || SystemSettings.debugMode > 0) printf ("\nFPS: %lf\n", txGetFPS());

		app->setColor (SystemSettings.BackgroundColor, txDC());
		app->rectangle (0, 0, app->systemSettings->FullSizeOfScreen.x, app->systemSettings->FullSizeOfScreen.y, txDC());

        Vector mp = {txMouseX (), txMouseY ()};
        manager->mousePos = mp;
        if (SystemSettings.debugMode > 0) printf("Engine getMBCondition(): %d\n", txMouseButtons());
        if (SystemSettings.debugMode > 0) printf("Engine mp: {%lf, %lf}\n", mp.x, mp.y);

		manager->draw ();
		if (manager->finalDC) app->bitBlt (txDC(), manager->rect.pos.x, manager->rect.pos.x, 0, 0, manager->finalDC);
        if (wasResizedInLastFrame) app->setResized(false);
        wasResizedInLastFrame = app->wasResized();


		if (txMouseButtons ())
		{
            manager->clicked = txMouseButtons();
			manager->onClick (mp);
			if (!IsRunning) break;
		}
        else
        {
            manager->clicked = 0;
        }

		
		txSleep (0);

	}
}


bool swapDC(HDC dest, int xDest, int yDest, int wDest, int hDest,
    HDC src, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rOp)
{
    return txBitBlt(dest, 0, 0, wDest, hDest, src);
}



void RECTangle (const Rect rect, HDC dc /* = txDc ()*/)
{
    assert (dc);
	//app->rectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}

void LaysMenu::onClick (Vector mp)
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
	app->rectangle(0, 0, SystemSettings.DCMAXSIZE, SystemSettings.DCMAXSIZE, finalDC);
	char text[30] = {};

    handle.print(finalDC);
    controlHandle();

    rect.finishPos.y = handle.rect.getSize().y + rect.pos.y;
	if (canvasManager->getActiveCanvas() != NULL)
	{
        rect.finishPos.y = handle.rect.getSize().y + rect.pos.y + sectionHeight * canvasManager->getActiveCanvas()->currentLayersLength + buttonSize.y;
        
		for (int i = 0; i < (canvasManager->getActiveCanvas())->currentLayersLength; i++)
		{

			sprintf(text, "���� %d", i + 1);

			app->setAlign(TA_CENTER, finalDC);
			app->setColor(SystemSettings.TextColor, finalDC);
            app->selectFont ("Arial", sectionFont, finalDC);
			
			app->drawText(sideThickness, sideThickness + handle.rect.getSize().y + sectionHeight * i, rect.getSize().x, handle.rect.getSize().y + sectionHeight * (i + 1), text, finalDC, DT_VCENTER);

			app->line(0, handle.rect.getSize().y + sectionHeight * (i), rect.getSize().x, handle.rect.getSize().y + sectionHeight * (i), finalDC);
		}
        app->bitBlt(finalDC, 0, rect.getSize().y - buttonSize.y, 0, 0, addNewLayButton);
        app->line(0, rect.getSize().y - buttonSize.y, rect.getSize().x, rect.getSize().y - buttonSize.y, finalDC);
	}
    setMbLastTime();
}

void TimeButton::draw ()
{
	if (manager)app->setColor (manager->color, finalDC);
	app->rectangle(0, 0, getSize().x, getSize().y, finalDC);
	time_t t = time (NULL);
	t = t % (24 * 3600);

	char newStr[50] = {};
	int hours =	t / 3600;
	int minutes = t / 60 - hours * 60;
	int second  = t - hours * 3600 - minutes * 60;

	sprintf (newStr, "%d:%02d:%02d", hours + 3, minutes, second);

	app->setAlign (TA_LEFT, finalDC);
	app->setColor (color, finalDC);
    app->selectFont("Arial", font, finalDC);
	app->drawText (0, 0, getSize().x, getSize().y, newStr, finalDC);
	//app->drawOnScreen(finalDC);

}



void SizeButton::draw ()
{
	$s
	//if (dc) app->bitBlt (rect.pos.x, rect.pos.y, dc);

	//txSetTextAlign (TA_CENTER);
		//app->setColor (TX_GRAY);
        /*
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 5, 
					text);    */
}

void CleanButton::draw()
{
	$s

	if (dc) copyOnDC (0, 0, dc);
		

	/*
	txSetTextAlign (TA_CENTER);
	app->setColor (TX_BLACK);  */
	
	/*
	txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 4, 
					text, finalDC);
					*/
}

void CloseButton::draw()
{
	$s
	//app->setColor (color);
	//app->rectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	app->bitBlt (finalDC, 0, 0, 0, 0, dc);
}



Canvas* CanvasManager::getActiveCanvas()
{
    return activeCanvas;
}


void CanvasManager::draw ()
{
   
	app->setColor (SystemSettings.BackgroundColor, finalDC);
	app->rectangle (0, 0, 3000, 3000, finalDC);

    standartManagerDraw (this);
    activeWindow = activeCanvas;
}

bool CanvasManager::addCanvas()
{
    return addWindow(activeCanvas = new Canvas(app, { .pos = {100, 50}, .finishPos = {newCanvasWindowSize.x + 100, newCanvasWindowSize.y + 50} }, loadManager, closeCanvasButton));
}

void CanvasManager::onClick(Vector mp)
{
    mousePos = mp;
    int clickedCellNum = standartManagerOnClick(this, mp);
    
    if (clickedCellNum >= 0) 
    {
        activeCanvas = (Canvas*)pointers [clickedCellNum];
        replaceWindow (clickedCellNum);
    }
}

void CanvasManager::deleteButton()
{
	//newCanvas.deleteButton();
	smartDeleteDC (closeCanvasButton);

	for (int i = 0; i < length; i++)
	{
		if (pointers[i]) pointers[i]->deleteButton(); 
	}
}

void ProgressBar::setProgress(double* total, double* current)
{	
	totalNum = total;
	currentNum = current;

};


void ProgressBar::draw()
{
	$s
	if(manager) app->setColor(manager->color, finalDC);
	app->rectangle (0, 0, rect.getSize().x, rect.getSize().y, finalDC);

	app->setColor(color, finalDC);
	if (totalNum && currentNum)
	{
		app->rectangle(0, 0, rect.getSize().x * (*currentNum / *totalNum), rect.getSize().y, finalDC);
	}
	//app->drawOnScreen(finalDC);
} 

void Menu::draw()
{
    if (dc) app->bitBlt(finalDC, 0, 0, 0, 0, dc);

    controlHandle();

    onUpdate();

    drawOnFinalDC(handle);

    for (int i = 0; i < currentSize; i++)
    {
        drawOneLine(i);
    }

    if (manager->getActiveWindow() != this && manager)
    {
        activeWindow = NULL;
    }

}

void Menu::onClick(Vector mp)
{
    int missClicked = true;
    clickHandle();

    mp.y -= handle.rect.finishPos.y;

  

    mousePos = mp;
    int mx = mp.x;
    int my = mp.y;
    



    if (advancedMode)
    {
        reDraw = true;
        missClicked = onClickLine(mp);
    }

    if (missClicked == true) activeWindow = NULL;
}


void Canvas::draw ()
{
    app->setColor(TX_BLACK, finalDC);
    app->rectangle(0, 0, 3000, 3000, finalDC);
	app->setColor (systemSettings->BackgroundColor, finalDC); 

    

    if (systemSettings->debugMode == 3)  printf("Clicked: %d\n", getMBCondition());


    cleanOutputLay();                                                                                                                      
    controlLay();

    drawLay();

    CLay* activeLay = getActiveLay();
    if (activeLay->lay.lay) controlTool();

    controlEditLay();

	if (app->getAsyncKeyState('Q'))
	{
		endtillkey('Q');
		//playHistory ();
		createLay();

	}

    if (app->getAsyncKeyState ('2'))
    {
        systemSettings->DrawingMode = 2;
    }

	
	if (manager->activeWindow != this) wasClicked = false;

	 
	//scrollBarVert.draw ();
	//app->bitBlt (finalDC, scrollBarVert.rect.pos.x, scrollBarVert.rect.pos.y, scrollBarVert.rect.getSize().x, scrollBarVert.rect.getSize().y, scrollBarVert.finalDC);
	//scrollBarHor.draw ();
	//app->bitBlt (finalDC, scrollBarHor.rect.pos.x, scrollBarHor.rect.pos.y, scrollBarHor.rect.getSize().x, scrollBarHor.rect.getSize().y, scrollBarHor.finalDC);

    drawCadre();

	controlHandle();
	drawOnFinalDC(handle);
	
	controlSize();                                   

	closeCanvas.rect.pos = {rect.getSize().x - systemSettings->MENUBUTTONSWIDTH,  0};
	closeCanvas.rect.finishPos = {rect.getSize().x, systemSettings->HANDLEHEIGHT};
	
	
	drawOnFinalDC(closeCanvas);


    DrawingModeLastTime = systemSettings->DrawingMode;

	
}



HDC Canvas::getImageForSaving()
{
    HDC notClearedDC = getActiveLay()->lay.lay;

    HDC clearedDC = app->createDIBSection(getActiveLay()->lay.laySize.x, getActiveLay()->lay.laySize.y);

    app->transparentBlt(clearedDC, 0, 0, 0, 0, notClearedDC);

    return clearedDC;   
    //�������� HDC ������� ������� ����� �������������
}


int Canvas::getActiveLayNum()
{
    return activeLayNum;
}


CLay* Canvas::getActiveLay()
{
    if (activeLayNum < 0) return NULL;
    return &(lay[activeLayNum]);
} 


int Canvas::getCurrentToolLengthOnActiveLay()
{
    if (!getActiveLay()) return 0;
    if (!getActiveLay()->getActiveToolLay()) return 0;
    int length = getActiveLay()->getCurrentSize() - 1;
    return length;
}


void Canvas::controlTool()
{
    if (!activeTool)
    {
        startTool();
    }

    CLay* clay = getActiveLay();
    assert(clay);
    ToolLay* toollay = clay->getActiveToolLay();
    assert(toollay);
    Tool* tool = toollay->getTool();
    bool isFinished = toollay->isFinished();

    
    setCurrentData();
    if (!isFinished)
    {
        if (isDrawingModeChanged()) changeTool();
        if (systemSettings->debugMode == 5) printf("Num:%d_IsFinished:%d", clay->getActiveToolLayNum(), isFinished);
        if (toollay->useTool(currentDate))
        {
            finishTool();
        }
    }
    if (getMBCondition() == 0)
    {
        toollay->setMBCondition(0);
    }
}

void Canvas::finishTool()
{
    activeTool = false;
    toolLays[currentToolLength - 1].tool->clicked = 0;
}

void Canvas::returnHistory (int stepsBack)
{
	if (!(stepsBack <= currentHistoryLength)) return; 
	//HDC historyDC = playHistoryDC();
    //printfDCS ();
	copyAndDelete(canvas, playHistoryDC (stepsBack));
    //txDeleteDC (hdc);
    //printfDCS ();
	//currentHistoryNumber - 1 -= stepsBack;
	//if (currentHistoryNumber - 1 < 0) currentHistoryNumber - 1 += HistoryLength;
//if (!(timeSavedHistory >= HistoryLength))
	{
		currentHistoryLength--;
	}	

	timeSavedHistory--;

}

void copyAndDelete (HDC dest, HDC source)
{
    copyDC (dest, source);
    txDeleteDC (source);
}
 
void Canvas::controlFilter ()
{
	//if (app->getAsyncKeyState(VK_LEFT)) SecondFilterValue++;
	//if (app->getAsyncKeyState(VK_RIGHT)) SecondFilterValue--;
	//if (app->getAsyncKeyState(VK_DOWN)) FirstFilterValue+=10;
	//if (app->getAsyncKeyState(VK_UP)) FirstFilterValue-=10;
    /*
	filter->algorithm = FilterAlgorithm;


	if (manager->activeWindow != this) return;
	if (confirmFilter)
	{
		confirmFilter = false;
		nonConfirmFilter = false;
		app->bitBlt(canvas, 0, 0, 0, 0, tempFilterDC);
	}

	if (((int)FirstFilterValue != (int)lastFirstFilterValue || (int)SecondFilterValue != (int)lastSecondFilterValue))
	{
		if (filter->lastX == 0 && filter->lastY == 0)
		{
			lastRecountFirstFilterValue = FirstFilterValue;
			lastRecountSecondFilterValue = SecondFilterValue;
		}
		reCountEnded = filter->reCount(nonConfirmFilter, SecondFilterValue, FirstFilterValue);
		if (reCountEnded)
		{
			if (!((int)FirstFilterValue != (int)lastRecountFirstFilterValue || (int)SecondFilterValue != (int)lastRecountSecondFilterValue))
			{
				lastFirstFilterValue = FirstFilterValue;
				lastSecondFilterValue = SecondFilterValue;
			}
			else
			{
				reCountEnded = false;
			}
		}
	}
    */

}
 
void Canvas::controlSize()
{
    /*
	app->setColor(SystemSettings.MenuColor, finalDC);
	app->rectangle (0, 0, SystemSettings.SIDETHICKNESS, rect.getSize().y, finalDC);
	app->rectangle (0, rect.getSize().y - SystemSettings.SIDETHICKNESS, rect.getSize().x, rect.getSize().y, finalDC);
	app->rectangle(rect.getSize().x - SystemSettings.SIDETHICKNESS, rect.getSize().y - SystemSettings.SIDETHICKNESS, rect.getSize().x, 0, finalDC);
	if (isResizing)
	{
		if (startResizingCursor.x != txMouseX() || startResizingCursor.y != txMouseY())
		{
			//printf ("1\n");
			scrollBarVert.resize ({.pos = {rect.getSize().x * 0.99 - 20, handle.rect.finishPos.y}, .finishPos = {rect.getSize().x * 0.99, rect.getSize().y * 0.99}});
			scrollBarHor.resize ({.pos = {rect.getSize().x * 0.01, rect.getSize().y * 0.99 - 20}, .finishPos = {rect.getSize().x * 0.99 - 20, rect.getSize().y * 0.99}});
		}

		rect.finishPos.x += txMouseX() - startResizingCursor.x;
		rect.finishPos.y += txMouseY() - startResizingCursor.y;
		startResizingCursor = { txMouseX(), txMouseY() };
	}
	if (getMBCondition() != 1) isResizing = false;
    */
}

void Canvas::controlSizeSliders ()
{
	//scrollBarVert.resize ({.pos = {rect.finishPos}});
}


void SizeButton::onClick (Vector mp)
{
	if (sizeType > 0)
	{
		(*num)++;
	}
	if (sizeType < 0 && *num > 0)
	{
		(*num)--;
	}
}

void CloseButton::onClick (Vector mp)
{
	IsRunning = false;
}

int min (int a, int b)
{
	if (a > b) return b;
	else return a;
}

void Canvas::saveHistory ()
{
	//app->bitBlt (history[currentHistoryNumber - 1], 0, 0, 0, 0, canvas);
	timeSavedHistory++;
	if (currentHistoryNumber < HistoryLength - 1) currentHistoryNumber++;
	else currentHistoryNumber = 0;
    //printf ("%d %d\n ", currentHistoryNumber - 1, (currentHistoryNumber - 1 < HistoryLength - 1));

    currentHistoryLength++;

	/*
	if (timeSavedHistory > HistoryLength - 1)
	{
		int newLastStep = currentHistoryNumber - 1;
		if (newLastStep >= HistoryLength)
		{
			newLastStep = 0;
		}
		lastSavedDC = playHistoryDC(9);
		app->drawOnScreen(lastSavedDC);
	}  */
	
	//history[addNewHistoryNum].toolsNum = SystemSettings.DrawingMode;
	//history[addNewHistoryNum].pos = {lastClick.x + canvasCoordinats.x,  lastClick.y + canvasCoordinats.y};
	//history[addNewHistoryNum].size = size;
	//history[addNewHistoryNum].color = SystemSettings.DrawColor;
	//history[addNewHistoryNum].thickness = lineThickness;
	
	
	
}

void Canvas::deleteHistory ()
{
	for (int i = 0; i < HistoryLength; i++)
	{
		//smartDeleteDC (history[i]);
	}
}

void Canvas::controlEditLay()
{
    if (!getActiveLay()) return;
    ToolLay* activeToolLay = getActiveLay()->getActiveToolLay();
    if (app->getAsyncKeyState('E') && currentToolLength > 0 && activeToolLay->isFinished())
    {
        while (app->getAsyncKeyState('E')) {};
        editingMode = !editingMode;
        activeToolLay->needRedraw();
    }
}

bool Canvas::controlLay ()
{
    /*
    ToolLay* activeToolLay = getActiveLay()->getActiveToolLay();
    if (app->getAsyncKeyState(VK_RIGHT) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {1, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState(VK_LEFT) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {-1, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState(VK_DOWN) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {0, 1};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState(VK_UP) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {0, -1};
        activeToolLay->needRedraw();
    }

    if (app->getAsyncKeyState('T') && currentToolLength > 0)
    {
        activeToolLay->size += { 0.01, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState('G') && currentToolLength > 0)
    {
        activeToolLay->size += {-0.01, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState('Y') && currentToolLength > 0)
    {
        activeToolLay->size += { 0, 0.01};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState('H') && currentToolLength > 0)
    {
        activeToolLay->size += { 0, -0.01};
        activeToolLay->needRedraw();
    }
    */
	return false;
}

void Canvas::cleanOutputLay()
{
    for (int i = 0; i < currentLayersLength; i++)
    {
        //if (lay[i].redrawStatus ()) app->bitBlt(lay[i].getOutPutDC(), 0, 0, 0, 0, lay[i].getPermanentDC());
        //txAlphaBlend(lay[i].outputLay, 0, 0, 0, 0, lay[i].lay);
        //lay[i].clean(lay[i].outputLay);
        //txTransparentBlt (lay[i].outputLay, 0, 0, 0, 0, lay[i].lay, 0, 0, SystemSettings.TRANSPARENTCOLOR);
        //app->bitBlt(0, 0, lay[i].tempLay);
        //while (app->getAsyncKeyState('G')) { txSleep(0); }
        
    }
}

void Canvas::drawLay()
{
    for (int lays = 0; lays < currentLayersLength; lays++)
    {
        if (lay[lays].redrawStatus())
        {
            lay[lays].redraw();
            lay[lays].noMoreRedraw();
        }

        if (editingMode && (lays == getActiveLayNum()))
        {
            lay[lays].getActiveToolLay()->setMBCondition(getMBCondition());
            lay[lays].editTool(currentDate);
        }
        
        app->transparentBlt(finalDC, lay[lays].lay.layCoordinats.x, lay[lays].lay.layCoordinats.y, 0, 0, lay[lays].lay.outputLay);
    }  
}


void Canvas::drawCadre()
{
    app->setColor(app->systemSettings->MenuColor, finalDC, 5);
    app->drawCadre({ .pos = {0,0}, .finishPos = getSize() }, finalDC);
}

void bitBlt (RGBQUAD *dest, int x, int y, int sizeX, int sizeY, RGBQUAD *source, int originalSizeX, int originalSizeY, int sourceSizeX, int sourceSizeY)
{
	for (int i = 0; i < min (sizeX, sourceSizeX); i++)
	{
		for (int j = 0; j < min (sizeY, sourceSizeY); j++)
		{	
			if (j + y < originalSizeY && i + x < originalSizeX && j + y >= 0 && i + x >= 0)
			{
				//if ( j == 249) _getch ();
				RGBQUAD *destCopy = &dest[i + (originalSizeY - (j + y) - 1) * originalSizeX + x];
				RGBQUAD *sourceCopy = &source[i + (sourceSizeY - j - 1) * sourceSizeX];
				
				sourceCopy->rgbReserved= 255;

				//if (sourceCopy->rgbReserved != 255) _getch ();

				destCopy->rgbRed = sourceCopy->rgbRed * ((float) sourceCopy->rgbReserved / 255.0);
				destCopy->rgbGreen = sourceCopy->rgbGreen * ((float)sourceCopy->rgbReserved / 255.0);
				destCopy->rgbBlue = sourceCopy->rgbBlue * ((float)sourceCopy->rgbReserved / 255.0);
				destCopy->rgbReserved = 255;

				//txSetPixel (i, j, RGB (sourceCopy->rgbRed, sourceCopy->rgbGreen, sourceCopy->rgbBlue));
				//txSetPixel (i, j, RGB (dest[i + (sizeY - (j + y) - 1) * sizeX + x].rgbRed, dest[i + (sizeY - (j + y) - 1) * sizeX + x].rgbGreen, dest[i + (sizeY - (j + y) - 1) * sizeX + x].rgbBlue));

				continue;
			}

		}
	}
}


void CToolManager::addTool (Tool *tool)
{
    assert (tool);
    if (currentLength < ToolsLength - 1)
    {
        tools[currentLength] = tool;
        currentLength++;    
    }

}



void Canvas::createLay ()
{
    assert(!(currentLayersLength >= LayersNum));
    lay[currentLayersLength].createLay(app);
    if (currentLayersLength <= LayersNum) currentLayersLength++;

    activeLayNum = currentLayersLength - 1;
}

HDC Canvas::playHistoryDC (int stepBack)
{
	/*
	if (timesShowedHistoryInRow == HistoryLength) return;
	if (currentHistoryLength <= 1) return; 
	if (currentHistoryNumber - 1 > 0)
	{
		//canvas = history[--currentHistoryNumber - 1];
		currentHistoryLength--;
	}
	else 
	{
		currentHistoryNumber - 1 = HistoryLength;
		//canvas = history[--currentHistoryNumber - 1];
		currentHistoryLength;
		
	}			  */
	HDC historyDC = app->createDIBSection (canvasSize.x, canvasSize.y);

	if (stepBack <= currentHistoryLength && stepBack < HistoryLength)
	{
		app->bitBlt(historyDC, 0, 0, 0, 0, lastSavedDC);
		//app->drawOnScreen(lastSavedDC);

		for (int i = currentHistoryLength - stepBack - 1; i >= 0; i--)
		{
			int pos;
			//if (currentHistoryLength == HistoryLength - 1)  pos = currentHistoryNumber - 1 + 1 + i;
			if (currentHistoryLength < HistoryLength - 1) pos = currentHistoryNumber - 1 - 1 - i;
			if (pos >= HistoryLength) pos -= HistoryLength;
           // history[pos].tools->load (history[pos].toolsData, historyDC);
		}
	}
	

	timesShowedHistoryInRow++;
	return	historyDC;
}

void Canvas::deleteButton ()
{
	if (dc) app->deleteDC (dc);
	if (finalDC) app->deleteDC(finalDC);
	if (canvas) app->deleteDC(canvas);
 	closeCanvas.deleteButton ();
	scrollBarVert.deleteButton ();
	scrollBarHor.deleteButton ();
	deleteHistory();
	
}

void Canvas::startTool()
{
    initToolLay();

    controlTool();
}

void Canvas::changeTool()
{
    delete (getActiveLay()->getActiveToolLay()->getToolsData());

    setToolToToolLay (getActiveLay()->getActiveToolLay());
}

void Canvas::initToolLay()
{
    
    currentDate->size = { lineThickness, lineThickness };
    addToolLay();
    getActiveLay()->addToolLay(&toolLays[currentToolLength]);

    currentToolLength++;
}

void Canvas::addToolLay()
{
    assert(LayersNum >= currentToolLength);
    activeTool = true;
    Tool* activeTool = getActiveTool();
    ToolLay* toollay = getNewToolLay();
    setToolToToolLay(toollay);
}


void Canvas::setToolToToolLay(ToolLay* toollay)
{
    toollay->tool = getActiveTool();
    if (toollay->tool == NULL) return;
    toollay->toolsData = new char[getActiveTool()->ToolSaveLen]{};
}

void Canvas::setCurrentData()
{
    currentDate->mousePos = getMousePos();
    currentDate->managerPos = getAbsCoordinats();
    currentDate->color = systemSettings->DrawColor;
    currentDate->canvasCoordinats = canvasCoordinats;
    currentDate->backGroundColor = TX_BLACK;
}

ToolLay* Canvas::getNewToolLay()
{
    return &(toolLays[currentToolLength]);
}

bool Canvas::isDrawingModeChanged()
{
    return DrawingModeLastTime != systemSettings->DrawingMode;
}

Tool* Canvas::getActiveTool()
{
    if (app->systemSettings->DrawingMode <= 0 || app->toolManager->getNumberOfLibs() <= 0) return NULL;
    return ToolManager.tools[app->systemSettings->DrawingMode - 1];
}

void Canvas::setActiveToolLayNum(int num) 
{ 
    getActiveLay()->setActiveToolLayNum(num); 
    if (getActiveLay()->getActiveToolLay()->isFinished()) editingMode = 1;
    else editingMode = 0;
};

int Canvas::getLastNotStartedToolNum()
{
    int length = getCurrentToolLengthOnActiveLay();
    return length;
}

void Canvas::onClick(Vector mp)
{
    if (getMBCondition() == 1)
    {

        ///app->setColor(drawColor, fi);
        lastClick = mp;
        mousePos = mp;


        int mx = mp.x;
        int my = mp.y;

        if (manager->activeWindow == this)
        {
            if (scrollBarVert.rect.inRect(mx, my))
            {
                clickButton(&scrollBarVert, this, mousePos);
                //scrollBarVert.onClick(mp - scrollBarVert.rect.pos);
                return;
            }
            if (scrollBarHor.rect.inRect(mx, my))
            {
                clickButton(&scrollBarHor, this, mousePos);
                //scrollBarHor.onClick(mp - scrollBarHor.rect.pos);
                return;
            }

            if (closeCanvas.rect.inRect(mx, my))
            {
                advancedMode = false;
                return;
            }
        }

        if (clickHandle()) return;

        if (!(isClickedLastTime()) && manager->activeWindow == this)
        {
            if (handle.rect.inRect(lastClick.x, lastClick.y))
            {
                if (resizingPlace.inRect(lastClick.x, lastClick.y))
                {
                    //startResizingCursor = { (double)mx, (double)my };
                    //isResizing = true;
                    return;
                }

                return;
            }

        }
    }

    if (editingMode)
    {
        if (getActiveLay()->getActiveToolLay()->isInToolZone(currentDate, getMousePos(), getMBCondition())) return;
    }

    
    //independet scenery block++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (getActiveLay() && getActiveLay()->getActiveToolLay()) getActiveLay()->getActiveToolLay()->setMBCondition(getMBCondition());
    //independet scenery block----------------------------------------------------
    

    setMbLastTime();
}

void ColorButton::onClick (Vector mp)
{
	SystemSettings.DrawColor = color;
}



void CleanButton::onClick (Vector mp)
{
	mainCanvas->clearBackground = true;
}


void ResizeButton::onClick(Vector mp)
{
    PowerPoint* fullapp = (PowerPoint*)app;

    if (!isClickedLastTime())
    {
        Vector saveSize = app->systemSettings->SizeOfScreen;
        if (app->isFullScreen())
        {
            if (app->systemSettings->FullSizeOfScreen == app->systemSettings->lastTimeSizeOfScreen || app->systemSettings->lastTimeSizeOfScreen == app->systemSettings->lastTimeSizeOfScreen.getNullVector())
            {
                Vector newSize = app->systemSettings->FullSizeOfScreen / 1.5;
                fullapp->changeWindow(newSize);
            }
            else
            {
                fullapp->changeWindow(app->systemSettings->lastTimeSizeOfScreen);
            }

        }
        else
        {
            fullapp->changeWindow(app->systemSettings->FullSizeOfScreen);
        }
        app->setResized();
        app->systemSettings->lastTimeSizeOfScreen = saveSize;
    }
}

void ResizeButton::draw()
{
    setMbLastTime();
    if (app->isFullScreen()) dc = nowIsFullScreen;
    else                     dc = nowIsNotFullScreen;


    standartDraw(this);
}



void invertDC(RGBQUAD* buf, unsigned int totalsize)
{
    for (int i = 0; i < totalsize; i++)
    {
        buf[i].rgbReserved = 255 - buf[i].rgbReserved;
    }
}
