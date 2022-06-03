
#define _CRT_SECURE_NO_WARNINGS
#include "GlobalOptions.h"
#include "DrawBibliothek.cpp"
#include "..\Macroses.h"
#include "..\Q_Vector.h"
#include "..\Q_Rect.h"
#include "Q_Buttons.h"
#include <cmath>
#include "C:\Users\Алехандро\Desktop\AlexProjects\Brightness\dllmain.h"
//#include "..\Brightness\Q_Filter.h"
#include "StandartFunctions.h"
#include "CurvesFilter.h"
#include "..\Graphicapp-dll.h"
#include "Slider.cpp"
#include "Canvas.h"
#include "TransferStructure.h"
#include "DLLFiltersManager.cpp"
#include "Tool.h"
#include "DLLToolsManager.cpp"
#include "CLay.cpp"
#include "MainTools.cpp"


//#include "LoadManager.h"
int testMode = 1;



HDC TestPhoto;

void bitBlt(RGBQUAD* dest, int x, int y, int sizeX, int sizeY, RGBQUAD* source, int originalSizeX = DCMAXSIZE, int originalSizeY = DCMAXSIZE, int sourceSizeX = DCMAXSIZE, int sourceSizeY = DCMAXSIZE);

void txSelectFontDC(const char* text, int size, HDC& dc);
void copyAndDelete (HDC dest, HDC source);
void invertDC (RGBQUAD* buf, unsigned int totalsize);


 




















CToolManager ToolManager;


struct Menu : Manager
{
    int lastSelected = 0;
    int currentSize = 0;
    Menu(Rect _rect, Rect _handle, int _length = ONEMENUBUTTONSNUM) :
        Manager(_rect, _length, false, NULL, _handle, MenuColor, true)
    {}

    virtual void drawOneLine(int lineNum) = NULL;
    virtual int onClickLine(Vector mp) = NULL;
    virtual void onUpdate() {};

    virtual void draw();
    virtual void onClick(Vector mp);


};




struct ToolsPalette : Menu
{
    
	ToolsPalette (Rect _rect, int _length) :
		Menu(_rect, { .pos = {0, 0}, .finishPos = {DCMAXSIZE, HANDLEHEIGHT}}, _length)
	{
        Window *tools = new Window[ToolManager.currentLength];
        for (int i = 0; i < ToolManager.currentLength; i++)
        {
            tools[i].rect = {.pos = {0, (double) i * 50}, .finishPos = {50, (double) (i + 1) * 50}};
            tools[i].dc = ToolManager.tools[i]->dc;
            tools[i].finalDC = txCreateDIBSection (tools[i].getSize().x, tools[i].getSize().y);
            tools[i].originalRect = tools[i].rect;
            //printBlt (tools[i].dc);
            addWindow (&tools[i]);
            currentSize++;
        }
        
	}

    virtual void drawOneLine(int lineNum);
    virtual int onClickLine(Vector mp);
};


//Меню, которое будет позволять управлять уже нарисованными на слое инструментами
struct ToolMenu : Menu
{
    struct CanvasManager* canvasManager = NULL;


    ToolMenu(CanvasManager* _canvasManager) :
        Menu({ .pos = {0, 300}, .finishPos = {100, ONELAYTOOLSLIMIT * BUTTONHEIGHT} }, { .pos = {0, 0}, .finishPos = {DCMAXSIZE, HANDLEHEIGHT} }, ONELAYTOOLSLIMIT),
        canvasManager(_canvasManager)
    {
        //currentSize = canvasManager->getActiveCanvas()->getCurrentToolLengthOnActiveLay();
        txSetAllColors(color, finalDC);
        txRectangle(0, 0, rect.finishPos.x, rect.finishPos.y, finalDC);
        font = 30;
        txSelectFontDC(TEXTNAME, font, finalDC);
    }


    virtual void onUpdate();
    virtual void drawOneLine(int lineNum);
    virtual int  onClickLine(Vector mp);

};
		



struct ProgressBar : Window
{
	double* totalNum = NULL;
	double* currentNum = NULL;

	ProgressBar (Rect _rect, COLORREF _color = TX_LIGHTGREEN) :
		Window(_rect, _color)
	{}

	void setProgress(double *total, double *current);

	virtual void draw() override;
};








struct TimeButton : Window
{
	int font;
	TimeButton (Rect _rect, COLORREF _color = TX_RED, int _font = MainFont)	:
		Window (_rect, _color),
		font (_font)
	{}

	virtual void draw () override;
};

struct CloseButton : Window
{ 

	CloseButton (Rect _rect, COLORREF _color, HDC _dc) :
		Window (_rect, _color, _dc)
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

struct ColorButton : Window
{
	ColorButton(Rect _rect, COLORREF _color, HDC _dc) :
		Window (_rect, _color, _dc)
	{
		draw ();
		reDraw = false;
	}

	virtual void onClick (Vector mp)override;
};
																																					    
struct SizeButton : Window
{
	double *num;
	int sizeType;

	SizeButton(Rect _rect, double *_num, int _sizeType) :
		Window (_rect),
		num (_num), 
		sizeType (_sizeType)
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;

};

struct CleanButton : Window
{
	Canvas *mainCanvas;

	CleanButton (Rect _rect, COLORREF _color, Canvas *_mainCanvas, HDC _dc = NULL, const char *_text = "") :
		Window (_rect, _color, _dc),
		mainCanvas (_mainCanvas)
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

struct CanvasManager : Manager 
{
	//Window newCanvas;
	HDC closeCanvasButton;
	//Vector sizeOfNewCanvas;
	Canvas *activeCanvas = NULL; 
	ProgressBar* bar;
    bool addNewCanvas = false;
    Vector newCanvasWindowSize = {1000, 700};
	//bool (*reCount)(Filter& filter, int screenSize, bool confirmSecondFilterValue, double SecondFilterValue, double FirstFilterValue);


	CanvasManager (Rect _rect, HDC _NewCanvasDC, ProgressBar* _bar) :
		Manager (_rect, 10, true, NULL, {}, TX_BLACK), 
		bar (_bar)
	{
		compressImage (closeCanvasButton, { MENUBUTTONSWIDTH,  HANDLEHEIGHT}, LoadManager.loadImage("CloseButton4.bmp"), {50, 26}, __LINE__);
	}

    Canvas* getActiveCanvas();

	virtual void draw () override;
	virtual void onClick(Vector mp) override;

	virtual void deleteButton() override;
};

struct History : Manager
{
	CanvasManager *canvasManager;
	ToolsPalette *palette;
	HDC openCanvas;
	int toolHDCSize;
	HDC toolsDC[TOOLSNUM];

	History (Rect _rect, CanvasManager *_canvasManager, ToolsPalette *_palette, HDC _openCanvas = NULL) :
		Manager (_rect, 0, true, NULL, {.pos = {0, 0}, .finishPos = {_rect.getSize().x, HANDLEHEIGHT}}),
		canvasManager (_canvasManager),
		palette (_palette),
		toolHDCSize (palette->pointers[0]->rect.getSize().y * 0.5)
	{
		txSelectFont ("Arial", 21, 7, FW_DONTCARE, false, false, false, 0, handle.finalDC);
		rect.finishPos.y = rect.pos.y + handle.rect.getSize().y;
		handle.rect.finishPos.x = rect.getSize().x;
		handle.color = MenuColor;
		handle.text = "История";

		for (int i = 0; i < TOOLSNUM; i++)
		{
			compressImage (toolsDC[i], {(double)toolHDCSize/2, (double)toolHDCSize/2}, palette->pointers[i]->dc, {palette->pointers[0]->rect.getSize().y, palette->pointers[0]->rect.getSize().y}, __LINE__);
			 //printBlt (toolsDC[i]);
		}
		
		if (_openCanvas) compressImage (openCanvas, {rect.getSize().x, (double) toolHDCSize}, _openCanvas, {50, 50}, __LINE__);
	}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

struct LaysMenu : Manager
{
	CanvasManager* canvasManager;
	HDC openCanvas;
	int sectionHeight = HANDLEHEIGHT;
    int sectionFont = MainFont * 0.9;
	//HDC toolsDC[TOOLSNUM];

	LaysMenu(Rect _rect, CanvasManager* _canvasManager) :
		Manager(_rect, 0, false, NULL, { .pos = {0, 0}, .finishPos = {_rect.getSize().x, HANDLEHEIGHT} }),
		canvasManager(_canvasManager)
	{
		//txSelectFont("Arial", 21, 7, FW_DONTCARE, false, false, false, 0, handle.finalDC);
		rect.finishPos.y = rect.pos.y + handle.rect.getSize().y;
		handle.rect.finishPos.x = rect.getSize().x;
		handle.color = color;
		handle.text = "Слои";
        handle.font = MainFont;

		for (int i = 0; i < TOOLSNUM; i++)
		{
			//compressImage(toolsDC[i], { (double)toolHDCSize / 2, (double)toolHDCSize / 2 }, palette->pointers[i]->dc, { palette->pointers[0]->rect.getSize().y, palette->pointers[0]->rect.getSize().y });
			//printBlt (toolsDC[i]);
		}
	}

	virtual void draw() override;
	virtual void onClick(Vector mp) override;
};


struct OpenManager : Window
{
	Manager *openManager;
    bool isOpeningAnotherList = false;
    //bool advancedModeIsSameWithOpenManager;

	OpenManager (Rect _rect, COLORREF _color, Manager *_manager, HDC _dc = NULL, const char *_text = "")	:
		Window (_rect, _color, _dc, NULL, _text),
		openManager (_manager)
	{
        format = DT_VCENTER;
    }

    OpenManager ()	:
        Window (),
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



	StringButton(Rect _rect, COLORREF _color, char *_redactingText, int _redactingTextLength, Manager *_manager, int _MaxNum = 20, bool _onlyNums = false) :
		Window(_rect, _color, NULL, _manager),
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

	NumChange (Rect _mainRect, Rect _stringRect, Rect _plusRect, Rect _minusRect, Rect _sliderRect, HDC _plusMinusDC, double _sliderQuadrateScale, int _numLength, double *_num) :
		Manager (_mainRect, 4),
		num (_num),
		stringButton (_stringRect, TX_WHITE, inText, _numLength, this, 20, true),
		plusNum (_plusRect, _num, +1),
		minusNum (_minusRect, _num, -1),
		plusMinusDC (_plusMinusDC),
		minNum (1),
		maxNum (20),
		slider (_sliderRect, _num, _sliderQuadrateScale, 1, 20) 
	{}

	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

struct BrightnessButton : Manager
{
	Slider SecondFilterValueSlider;
	Slider FirstFilterValueSlider;
	Window closeButton;
	Window grafic;
	Window confirmButton;
	double graficScale;
	double copyOfSecondFilterValue = SecondFilterValue;
	double copyOfFirstFilterValue = FirstFilterValue;
	//double copyOfSecondFilterValue;

	BrightnessButton (Rect _mainRect, Rect _graficRect, Rect _sliderRect, Rect _FirstFilterValueSliderRect, Rect _confirmButton, Rect _closeButtonRect = {}) :
		Manager (_mainRect, 5, true, NULL, {.pos = {0, 0}, .finishPos = {_mainRect.getSize ().x, _mainRect.getSize ().y * 0.1}}),
		grafic (_graficRect),
		SecondFilterValueSlider (_sliderRect, &copyOfSecondFilterValue, 0.18, 0, 255, false, true),
		confirmButton (_confirmButton),
		closeButton (_closeButtonRect),
		FirstFilterValueSlider (_FirstFilterValueSliderRect, &copyOfFirstFilterValue, 0.18, 0, 255, false, true)
		{
			SecondFilterValueSlider.manager = this;
			FirstFilterValueSlider.manager = this;
			closeButton.manager = this;
			SecondFilterValueSlider.manager = this;
			confirmButton.manager = this;
			graficScale = (grafic.rect.getSize().y / grafic.rect.getSize().x);	

			compressImage (dc, {rect.getSize ().x, rect.getSize ().y}, LoadManager.loadImage ("Brightness.bmp"), {444, 361}, __LINE__);

			//printBlt (dc);
		}


	virtual void draw () override;
	virtual void onClick (Vector mp) override;
};

 


struct StatusBar : Manager
{

	TimeButton *timeButton = NULL;
	ProgressBar *progressBar = NULL;

	StatusBar(Rect _rect, COLORREF _color) :
		Manager(_rect, 3, true, NULL, {}, _color)
	{}


	virtual void draw() override;
};

struct TouchButton : Window
{
    bool *flag = NULL;

    TouchButton (Rect _rect, HDC _dc, bool *_flag) :
        Window (_rect, MenuColor, _dc),
        flag (_flag)
    {
    }

    virtual void onClick (Vector mp) override;
};

struct List : Manager
{
    int itemHeight = HANDLEHEIGHT;
    OpenManager *items;
    bool *isThisItemList;
    int lastClickedItemNum = -1;
    bool mayFewWindowsBeOpenedAtTheSameTime;
    int activeItemCircleSize = 3;

    List (Vector _pos, Vector _size, int _length, bool _mayFewWindowsBeOpenedAtTheSameTime = true) : 
        Manager ({.pos = _pos, .finishPos = {_pos.x + _size.x, _pos.y + _length * _size.y }}, _length, false),
        mayFewWindowsBeOpenedAtTheSameTime (_mayFewWindowsBeOpenedAtTheSameTime)
    {
        items = new OpenManager[length]{};
        isThisItemList = new bool[length]{};
        
        for (int i = 0; i < length; i++)
        {
            //items[i].advancedModeIsSameWithOpenManager = true;
        }
    }

    List (int _length, bool _mayFewWindowsBeOpenedAtTheSameTime = true) :
        Manager ({}, _length, false),
        mayFewWindowsBeOpenedAtTheSameTime (_mayFewWindowsBeOpenedAtTheSameTime)
    {
        items = new OpenManager[length]{};
        isThisItemList = new bool[length]{};
    }

    void addNewItem (Window *openButton, HDC dc = NULL, const char *text = NULL);
    Vector getNewSubItemCoordinats ();
    void addSubList (List *subList, const char *ListText);

    virtual void draw() override;
	virtual void onClick(Vector mp) override;
};

struct PowerPoint : AbstractAppData
{
    virtual void setColor (COLORREF color, HDC dc, int thickness);
    virtual void rectangle (Rect rect, HDC dc);
    virtual void drawOnScreen (HDC dc, bool useAlpha = false);
    virtual void cleanTransparentDC();
    virtual bool getAsyncKeyState(int symbol);
    virtual void deleteTransparency(RGBQUAD* buf, unsigned int totalSize);
    
};



bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;

void Engine (Manager *manager);

void RECTangle (const Rect rect, HDC dc = txDC ());
void shiftArrBack    (char arr[], int length);
void shiftArrForward (char arr[], int length);
bool checkDeltaTime (int lastTimeClicked);
void printfDCS (const char *str = "");




int main ()
{
	//_txWindowStyle &= ~WS_CAPTION;
	MAINWINDOW = txCreateWindow (SCREENSIZE.x, SCREENSIZE.y);

    TransferData Data;
    Data.MAINWINDOW = MAINWINDOW;
    Data.MainWindowDC = txDC ();


	Manager *manager = new Manager({.pos = {0, 0}, .finishPos = {SCREENSIZE.x, SCREENSIZE.y}}, 20, true, NULL, {}, TX_RED);

    ToolSave toolSave = {};

	HDC addNewCanvasDC = {};
	HDC oldDC = LoadManager.loadImage ("addNewCanvas.bmp");
	compressImage (addNewCanvasDC, {50, 50}, oldDC, {225, 225});
	txDeleteDC(oldDC);

	StatusBar* statusBar = new StatusBar( {.pos = {0, SCREENSIZE.y - 20}, .finishPos = {SCREENSIZE.x, SCREENSIZE.y}} , TX_BLUE);
		statusBar->progressBar =  new ProgressBar( {.pos = {0, 0}, .finishPos = statusBar->rect.getSize()}, TX_GREEN);
		statusBar->progressBar->manager = statusBar;
		statusBar->timeButton = new TimeButton({.pos = {statusBar->rect.getSize().x - 65, 0}, .finishPos = statusBar->rect.getSize()}, TX_WHITE);
		statusBar->timeButton->manager = statusBar;
    

	CanvasManager *canvasManager = new CanvasManager ({.pos = {0, 0}, .finishPos = {SCREENSIZE.x, SCREENSIZE.y}}, addNewCanvasDC, statusBar->progressBar);
	manager->addWindow (canvasManager);

    manager->addWindow(statusBar);


    PowerPoint* appData = new PowerPoint;
    appData->canvasManager = canvasManager;
    appData->currColor = &DrawColor;
    appData->transparentLay.lay = txCreateDIBSection(2000, 2000, &appData->transparentLay.layBuf);
    appData->transparentLay.laySize = { 2000, 2000 };
    appData->cleanTransparentDC();
    appData->invert = invertDC;
    txAlphaBlend(0, 0, appData->transparentLay.lay);

    appData->drawOnScreen(appData->transparentLay.lay);

    DLLToolsManager* dllToolsManager = new DLLToolsManager("DLLPathList.txt", appData);
    dllToolsManager->loadLibs();
    dllToolsManager->addToManager(&ToolManager);

    


	ToolsPalette *toolsPallette = new ToolsPalette({.pos = {0, 100}, .finishPos = {50, (double)ToolManager.currentLength * 50 + HANDLEHEIGHT + 100}}, ToolManager.currentLength);
    manager->addWindow(toolsPallette);
    ToolMenu* toolMenu = new ToolMenu(canvasManager);
    manager->addWindow(toolMenu);



	 
	Manager *menu = new Manager({.pos = {1488, 100}, .finishPos = {1900, 400}}, 3, false, LoadManager.loadImage ("HUD-4.bmp"), {.pos = {0, 0}, .finishPos = {412, 50}});
	manager->addWindow (menu);
	
	Manager *colorManager = new Manager({.pos = {10, 180}, .finishPos = {170, 220}}, 3, true, NULL, {}, RGB (26, 29, 29));
		menu->addWindow (colorManager);

			ColorButton *redColor = new ColorButton({.pos = {0, 0}, .finishPos = {40, 40}}, RGB (255, 0, 0), LoadManager.loadImage ("RedButton.bmp"));
			colorManager->addWindow(redColor);
	
			ColorButton *blueColor = new ColorButton({.pos = {60, 0}, .finishPos = {100, 40}}, RGB (0, 0, 255), LoadManager.loadImage ("BlueButton.bmp"));
			colorManager->addWindow(blueColor);

			ColorButton *greenColor = new ColorButton({.pos = {120, 0}, .finishPos = {160, 40}}, RGB (0, 255, 0), LoadManager.loadImage ("GreenButton.bmp"));
			colorManager->addWindow(greenColor);

	OpenManager *openManager = new OpenManager({.pos = {15, 135}, .finishPos = {36, 153}}, TX_WHITE, colorManager, LoadManager.loadImage ("OpenColorButton.bmp"));
	menu->addWindow (openManager);	
	

    

    //HMODULE filtersLibrary = LoadLibrary ("Debug\\DLLFilters.dll");
    //assert (filtersLibrary);


    DLLFiltersManager* dllManager = new DLLFiltersManager("DLLPathList.txt", appData);
    dllManager->loadLibs ();
    dllManager->addToManager(manager);

    LaysMenu* laysMenu = new LaysMenu ({.pos = {0, 700}, .finishPos = {BUTTONWIDTH, 1000}}, canvasManager);
    manager->addWindow(laysMenu);

    Curves *curves = new Curves ({.pos = {500, 500}, .finishPos = {500 + 443, 500 + 360}}, LoadManager.loadImage("Brightness.bmp"));
    manager->addWindow(curves);

	Manager* mainhandle = new Manager({ .pos = {0, 0}, .finishPos = {SCREENSIZE.x, HANDLEHEIGHT} }, 3, true, NULL, {}, RGB(45, 45, 45));
    manager->addWindow(mainhandle);

		CloseButton* closeButton = new CloseButton({ .pos = {SCREENSIZE.x - BUTTONWIDTH, 0}, .finishPos = {SCREENSIZE.x, HANDLEHEIGHT} }, TX_RED, LoadManager.loadImage("CloseButton4.bmp"));
		mainhandle->addWindow(closeButton);

        TouchButton *addNewCanvas = new TouchButton({.pos = {0, 0}, .finishPos = {BUTTONWIDTH, HANDLEHEIGHT}}, LoadManager.loadImage ("AddNewCanvas2.bmp"), &canvasManager->addNewCanvas);
		mainhandle->addWindow(addNewCanvas);

        List *openWindows = new List ({BUTTONWIDTH, HANDLEHEIGHT}, {BUTTONWIDTH * 6, HANDLEHEIGHT}, 5); 
        OpenManager *openWindowsManager = new OpenManager ({.pos = {BUTTONWIDTH, 0}, .finishPos = {BUTTONWIDTH * 2, HANDLEHEIGHT}}, TX_WHITE, openWindows, LoadManager.loadImage ("OpenWindows.bmp"));
        mainhandle->addWindow(openWindowsManager);
        
    manager->addWindow (openWindows);
        openWindows->addNewItem (menu, NULL, "Цвет");

        openWindows->addNewItem (toolsPallette, NULL, "Инструменты");
        openWindows->addNewItem (laysMenu, NULL, "Слои");
        openWindows->addNewItem (toolMenu, NULL, "Инструментальные слои");
         ///List *filters = new List (openWindows->getNewSubItemCoordinats(), {openWindows->getSize().x, openWindows->getSize().y/openWindows->length}, 3);
        List *filters = new List (1 + dllManager->currLoadWindowNum, false);
        manager->addWindow (filters);

        openWindows->addSubList (filters, "Фильтры");
            //filters->addNewItem (dllManager->dllWindows[0], NULL, "Контрастный фильтр");
            //filters->addNewItem (dllManager->dllWindows[1], NULL, "Фильтр яркости");
            filters->addNewItem (curves, NULL, "Кривые");
            for (int i = 0; i < dllManager->currLoadWindowNum; i++)
            {
                filters->addNewItem(dllManager->dllWindows[i], NULL, dllManager->dllWindows[i]->name);
            }
         

        /* 
         List *Filters = new List ({.pos       = {openWindows->rect.finishPos.x,               openWindows->rect.pos.y + openWindows->newButtonNum * openWindows->itemHeight},
                                    .finishPos = {openWindows->rect.finishPos.x + BUTTONWIDTH, openWindows->rect.pos.y + (openWindows->newButtonNum + 2) * openWindows->itemHeight}});
         openWindows->addNewItem (Filters, NULL, "Фильтры");
          */
         //OpenManager *contrastFilter = new OpenManager ({ .pos = {0, HANDLEHEIGHT}, .finishPos = {openWindows->getSize().x, HANDLEHEIGHT * 2} }, MenuColor, contrastMenu, NULL, "Контрастный фильтр");
         //openWindows->addNewItem (contrastMenu, NULL, "Контрастный фильтр");  

         //OpenManager *brightnessFilter = new OpenManager ({ .pos = {0, HANDLEHEIGHT * 2}, .finishPos = {openWindows->getSize().x, HANDLEHEIGHT * 3} }, MenuColor, brightnessMenu, NULL, "Фильтр яркости");
         //openWindows->addNewItem (brightnessMenu, NULL, "Фильтр яркости");

         //OpenManager *tools = new OpenManager ({ .pos = {0, HANDLEHEIGHT * 3}, .finishPos = {openWindows->getSize().x, HANDLEHEIGHT * 4} }, MenuColor, toolsPallette, NULL, "Инструменты");
         

	  
	 
	/*
	BrightnessButton *brightnessButton = new BrightnessButton ({.pos = {800, 100}, .finishPos = {1244, 461}}, {.pos = {55, 64}, .finishPos = {312, 318}},
															   {.pos = {322, 64}, .finishPos = {338, 318}}, {.pos = {17, 64}, .finishPos = {33, 318}},
															   {.pos = {357, 63}, .finishPos = {436, 80}},  
															   {.pos = {357, 90}, .finishPos = {436, 100}});	

	manager->addWindow(brightnessButton);
	*/

    
	
	//algorithm = (RGBQUAD(*)(RGBQUAD pixel, double SecondFilterValue, double FirstFilterValue))GetProcAddress(library, "KontrastFilter");
	//assert(algorithm);
	
	
	txBegin ();



	Engine (manager);

	txEnd ();

	//redColor->deleteButton ();
	//blueColor->deleteButton ();
	//blueColor->deleteButton ();

    delete manager;
    LoadManager.deleteAllImages ();
	 

	//menu->deleteButton ();
	//colorManager->deleteButton ();
	//closeButton->deleteButton ();

	/*
	txDeleteDC (redColor->dc);
	txDeleteDC (blueColor->dc);
	txDeleteDC (greenColor->dc);

	//txDeleteDC (numChange->plusMinusDC);
	numChange->slider.deleteDC ();
	brightnessButton->brightnessSlider.deleteDC ();
	txDeleteDC (brightnessButton->dc);
	txDeleteDC (brightnessButtonOpen->dc);

	txDeleteDC (cleanButton->dc);
	txDeleteDC (menu->dc); 
	txDeleteDC (openManager->dc);
	txDeleteDC (closeButton->dc);
	//txDeleteDC (xCoordinat->dc);
	//xCoordinat->deleteDC ();
	yCoordinat->deleteDC ();
	*/
	txDisableAutoPause ();

	return 0;


}


void PowerPoint::setColor (COLORREF color, HDC dc, int thickness)
{
    if (testMode) printf("SetColor: %d|", color);
    txSetAllColors (color, dc, thickness);
}

void PowerPoint::rectangle (Rect rect, HDC dc)
{
    txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}

void PowerPoint::drawOnScreen (HDC dc, bool useAlpha /*=false*/)
{
    if (!useAlpha)txBitBlt (0, 0, dc);
    if (useAlpha)txAlphaBlend (0, 0, dc);
    txSleep (0);
} 
void PowerPoint::cleanTransparentDC()
{
    txSetAllColors(TRANSPARENTCOLOR, transparentLay.lay);
    txRectangle(0, 0, transparentLay.laySize.x, transparentLay.laySize.y, transparentLay.lay);
    //txClear(transparentLay.lay);
    //deleteTransparency(transparentLay.layBuf, transparentLay.laySize.x * transparentLay.laySize.y);
}

bool PowerPoint::getAsyncKeyState(int symbol)
{
    return txGetAsyncKeyState(symbol);
}

void PowerPoint::deleteTransparency(RGBQUAD* buf, unsigned int totalSize)
{
    for (int i = 0; i < totalSize; i++)
    {
        buf[i].rgbReserved = 255;
    }
}


void List::draw()
{
    standartManagerDraw (this);

    for (int i = 0; i < newButtonNum; i++)
    {
        //pointers[i]->advancedMode = advancedMode;
        txSetAllColors (SecondMenuColor, finalDC, SIDETHICKNESS);
        txLine (0, i * itemHeight, rect.getSize().x, i * itemHeight, finalDC);

        if (items[i].openManager->advancedMode) 
            txEllipse (rect.getSize().x * 0.9 - activeItemCircleSize, ((double)i + 0.5) * itemHeight - activeItemCircleSize, rect.getSize().x * 0.9 + activeItemCircleSize, ((double)i + 0.5) * itemHeight + activeItemCircleSize, finalDC);

        if (isThisItemList[i] && !advancedMode)
        {
            items[i].openManager->advancedMode = false;
        }
    }
}

void List::onClick (Vector mp)
{
    mousePos = mp;
    int clikedButtonNum = standartManagerOnClick (this, mp);
    if (clikedButtonNum >= 0 && clikedButtonNum != lastClickedItemNum)
    {
        //printf ("last: %d, current: %d\n", lastClickedItemNum, clikedButtonNum);
        if (lastClickedItemNum >= 0 && items[lastClickedItemNum].openManager->advancedMode && !mayFewWindowsBeOpenedAtTheSameTime) clickButton (pointers[lastClickedItemNum], this, mp);
        lastClickedItemNum = clikedButtonNum;
    }

}

void StatusBar::draw()
{
	txSetAllColors(color, finalDC);
	txRectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);

	if (timeButton)timeButton->draw();
	
	progressBar->draw();

	int currentNum = NULL;
	if (progressBar->currentNum)
	{
		currentNum = *progressBar->currentNum;
	}

	if (currentNum)txBitBlt(finalDC, 0, 0, rect.getSize().x, rect.getSize().y, progressBar->finalDC);

	char text[32] = {};
	txSetAllColors(TX_WHITE, finalDC);
	txSelectFontDC("Arial", MainFont, finalDC);
	if (currentNum != 0)
	{
		sprintf(text, "Статус: применяется фильтр");
	}

	if (currentNum == 0)
	{
		sprintf(text, "Статус: фоновых задач нет");
	}
	txSetTextAlign(TA_LEFT, finalDC);
	txTextOut(10, 0, text, finalDC);

	txBitBlt(finalDC, timeButton->rect.pos.x, timeButton->rect.pos.y, timeButton->getSize().x, timeButton->getSize().y, timeButton->finalDC);

}

void List::addNewItem (Window *openButton, HDC dc/* = NULL*/, const char *text/* = NULL*/)
{
    Rect newRect = {.pos = {0, (double)(newButtonNum) * itemHeight}, .finishPos = {rect.getSize().x, (double)(newButtonNum + 1) * itemHeight}};
    items[newButtonNum].rect =  newRect;
    items[newButtonNum].color =  MenuColor;
    items[newButtonNum].openManager =  (Manager*)openButton;
    items[newButtonNum].dc =  dc;
    items[newButtonNum].text =  text;
    items[newButtonNum].reInit ();


    addWindow (&items[newButtonNum]);
}


void List::addSubList (List *subList, const char *ListText)
{
    subList->rect = {.pos = getNewSubItemCoordinats (), .finishPos = {getNewSubItemCoordinats ().x + rect.getSize().x, getNewSubItemCoordinats ().y + rect.getSize().y / length * subList->length}};
    subList->resize(subList->rect);

    isThisItemList[newButtonNum] = true;
    addNewItem (subList, NULL, ListText);
}

Vector List::getNewSubItemCoordinats ()
{
    return Vector {rect.finishPos.x, rect.pos.y + (double)(newButtonNum) * itemHeight}; 
}


void TouchButton::onClick (Vector mp)
{
    if (!isClicked) *flag = true;
}

void txSelectFontDC(const char* text, int size, HDC &dc)
{
	txSelectFont(text, size, -1, FW_DONTCARE, false, false, false, 0, dc);
}



void BrightnessButton::draw ()
{
	if (!advancedMode) return;

	if (txMouseButtons() != 1)
	{
		SecondFilterValue = copyOfSecondFilterValue;
		FirstFilterValue = copyOfFirstFilterValue;
	}

	controlHandle ();

	if (dc) copyOnDC (0, 0, dc);
	SecondFilterValueSlider.draw ();
	//printBlt(brightnessSlider.finalDC);
	txBitBlt (finalDC, SecondFilterValueSlider.rect.pos.x, SecondFilterValueSlider.rect.pos.y, SecondFilterValueSlider.rect.getSize().x, SecondFilterValueSlider.rect.getSize().y, SecondFilterValueSlider.finalDC);

	FirstFilterValueSlider.draw ();
	//printBlt(FirstFilterValueSlider.finalDC);
	txBitBlt (finalDC, FirstFilterValueSlider.rect.pos.x, FirstFilterValueSlider.rect.pos.y, FirstFilterValueSlider.rect.getSize().x, FirstFilterValueSlider.rect.getSize().y, FirstFilterValueSlider.finalDC);
	//txBitBlt (FirstFilterValueSlider.rect.pos.x, FirstFilterValueSlider.rect.pos.y, FirstFilterValueSlider.finalDC);
	//txBitBlt (300, 300, FirstFilterValueSlider.finalDC);
	txSetFillColor (TX_RED);
	txRectangle (300, 300, 400, 400);

	txSetAllColors (BackgroundColor, finalDC);
	txLine (grafic.rect.pos.x, grafic.rect.pos.y + grafic.rect.getSize ().y * (copyOfFirstFilterValue / 255),
			grafic.rect.finishPos.x, grafic.rect.pos.y + grafic.rect.getSize ().y * (copyOfSecondFilterValue / 255), finalDC);
}

void BrightnessButton::onClick (Vector mp)
{
	if (!advancedMode) return;
	int mx = mp.x;
	int my = mp.y;

	if (handle.rect.inRect (mx, my))
	{
			startCursorPos.x = mx;
			startCursorPos.y = my;
			handle.isClicked = true;
	}

	if (SecondFilterValueSlider.rect.inRect (mx, my) && !isClicked) SecondFilterValueSlider.onClick (mp - SecondFilterValueSlider.rect.pos);		
	if (FirstFilterValueSlider.rect.inRect (mx, my) && !isClicked) FirstFilterValueSlider.onClick (mp - FirstFilterValueSlider.rect.pos);		
	if (closeButton.rect.inRect (mx, my) && !isClicked) advancedMode = false;
	if (confirmButton.rect.inRect (mx, my) && !isClicked) 
	{
		confirmFilter = true;
		//SecondFilterValueSlider.maxNum = Brightness;
		//brightnessSlider.maxNum = 255;
		//FirstFilterValueSlider.maxNum = FirstFilterValue;
	}
}


void NumChange::draw ()
{
	if (! (*num == 0))
		sprintf (inText, "%d", (int) *num);

	stringButton.draw ();

	if (!strcmp ("", inText))
		*num = 0;

	sscanf  (inText, "%lf", num);

	txBitBlt (plusNum.rect.pos.x, plusNum.rect.pos.y, plusMinusDC); 

	//if (backGround) txBitBlt (minusNum.rect.pos.x, minusNum.rect.pos.y, backGround);

	plusNum.draw  ();
	minusNum.draw (); 

	int numBeforeSlider = *num;
	slider.draw ();
	//checkTextLen (*num, numBeforeSlider, &stringButton.textLen, &stringButton.cursorPosition);

	if (clicked != 1 && plusNum.isClicked  == true) plusNum.isClicked = false;
	if (clicked != 1 && minusNum.isClicked == true) minusNum.isClicked = false;

	if (manager->activeWindow != this) activeWindow = NULL;
}


void NumChange::onClick (Vector mp)
{
	int mx = txMouseX ();
	int my = txMouseY ();

	 if (stringButton.rect.inRect (mx, my))
	 {
		 activeWindow = &stringButton;
	 }

	if (plusNum.rect.inRect (mx, my) && !plusNum.isClicked)
	{
		activeWindow = &plusNum;

		if (*num >= maxNum) return;
		if ((*num == 9) || (*num == 99) || (*num == 999) || (*num == 9999) && !plusNum.isClicked && *num < maxNum) //fix?!
		{
			stringButton.textLen++;
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		if (!plusNum.isClicked)
		{
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		//plusNum.onClick ();
		plusNum.isClicked = true;
	}

	if (minusNum.rect.inRect (mx, my) && !minusNum.isClicked)
	{
		activeWindow = &minusNum;

		if (*num <= minNum) return;

		if ((*num == 10) || (*num == 100) || (*num == 1000) || (*num == 10000)&& !plusNum.isClicked && minNum < *num)
		{
			stringButton.textLen--;
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		if (!plusNum.isClicked)
		{
			stringButton.cursorPosition =  stringButton.textLen - 2;
		}
		//minusNum.onClick ();
		minusNum.isClicked = true;
	}


	if (slider.rect.inRect (mx, my))
	{
		activeWindow = &slider;

		bool changeTextLen = false;
		int numBeforeSlider = *num;

		//slider.onClick();

		//checkTextLen (*num, numBeforeSlider, &stringButton.textLen, &stringButton.cursorPosition);
	}

}

void ToolsPalette::drawOneLine(int lineNum)
{
    pointers[lineNum]->draw();
    //printBlt (pointers[lineNum]->dc);
    if (pointers[lineNum]->advancedMode) txBitBlt(finalDC, pointers[lineNum]->rect.pos.x, pointers[lineNum]->rect.pos.y + handle.rect.finishPos.y, pointers[lineNum]->rect.finishPos.x, pointers[lineNum]->rect.finishPos.y, pointers[lineNum]->finalDC);

    if (lastSelected == lineNum)
    {
        txSetAllColors(TX_WHITE, finalDC);
        txRectangle(pointers[lineNum]->rect.pos.x, pointers[lineNum]->rect.pos.y + handle.rect.finishPos.y, pointers[lineNum]->rect.pos.x + pointers[lineNum]->rect.getSize().x * 0.1, pointers[lineNum]->rect.pos.y + pointers[lineNum]->rect.getSize().y * 0.1 + handle.rect.finishPos.y, finalDC);
    }
    if (txMouseButtons() != 1)
    {
        pointers[lineNum]->isClicked = false;
    }

    txSetColor(TX_BLACK, 1, finalDC);
    txLine(0, pointers[lineNum]->rect.pos.y + handle.rect.getSize().y, rect.getSize().x, pointers[lineNum]->rect.pos.y + handle.rect.getSize().y, finalDC);
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
            //pointers[lineNum]->isClicked = true;
            DrawingMode = lineNum + 1;
            lastSelected = lineNum;

            missClicked = false;

            if (pointers[lineNum]->advancedMode) return missClicked + 2;
        }
        else
        {
            pointers[lineNum]->isClicked = false;

            missClicked = true;
        }

        
    }
    return  1;
}

void ToolMenu::onUpdate()
{   
    Canvas* activeCanvas = canvasManager->getActiveCanvas();

    if (activeCanvas) currentSize = activeCanvas->getCurrentToolLengthOnActiveLay();
    rect.finishPos.y = currentSize * BUTTONHEIGHT + handle.rect.finishPos.y + rect.pos.y;
}


void ToolMenu::drawOneLine(int lineNum)
{
    CLay* lay = canvasManager->getActiveCanvas()->getActiveLay();
    Tool* tool = lay->getToolLays()[lineNum]->getTool();
    HDC toolDC = tool->getDC(); 
    lastSelected = lay->getActiveToolLayNum();

    char outputText[20] = {};
    sprintf(outputText, "%d", lineNum + 1);

    int linePosY = BUTTONHEIGHT * lineNum + handle.rect.finishPos.y;

    txBitBlt(finalDC, 0, linePosY, BUTTONWIDTH, linePosY + BUTTONHEIGHT, toolDC);
    txSetColor(TextColor, 1, finalDC);

    txDrawText(BUTTONWIDTH, linePosY, rect.getSize().x, linePosY + BUTTONHEIGHT, outputText, TEXTFORMAT, finalDC);

    txSetColor(TX_BLACK, 1, finalDC);
    txLine(0, linePosY, rect.getSize().x, linePosY, finalDC);

    if (lastSelected == lineNum)
    {
        txSetAllColors(TX_WHITE, finalDC);
        txRectangle(0, linePosY, 5, linePosY + 5, finalDC);
    }
}


int ToolMenu::onClickLine(Vector mp)
{
    int my = mp.y;

    int buttonNum = my / BUTTONWIDTH;


    if (!(buttonNum >= 0 && buttonNum <= currentSize))  return 0;

    canvasManager->getActiveCanvas()->getActiveLay()->setActiveToolLayNum(buttonNum);

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
			if (txGetAsyncKeyState(VK_LEFT) && cursorPosition >= 0)
			{
				cursorMovement (VK_LEFT);
				//printf ("left");
				switched = true;
			}
			if (txGetAsyncKeyState(VK_RIGHT) && cursorPosition <= textLen - 2)
			{
				cursorMovement (VK_RIGHT);
				switched = true;
			}

			if (txGetAsyncKeyState (VK_BACK))
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

	txSetTextAlign (TA_LEFT);
	txSetAllColors (TX_WHITE);
	
	txTextOut (rect.pos.x, rect.pos.y, inText);

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
	return clock () - lastTimeClicked > DELTACLOCKTIME;
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

	if (validMovement && clock () - lastTimeClicked > DELTACLOCKTIME)
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
	if (!isClicked)
	{
		openManager->advancedMode = !openManager->advancedMode;
		openManager->draw ();
	}
}

void OpenManager::draw()
{
    standartDraw (this);
	txSetAllColors (TextColor, finalDC, MainFont); 
    
    //openManager->advancedMode = advancedMode;

    //txTriangle (rect.getSize().x - 10, rect.getSize().y * 0.3,  rect.getSize().x - 10, rect.getSize().y * 0.66, rect.getSize().x - 5, rect.getSize().y * 0.5);
		//txSelectFont ("Arial", 40);
	//if (text) txTextOut (0, 0,  text, finalDC,);
	//if (text) txDrawText (0, 0, rect.getSize ().x, rect.getSize ().y, text, DT_VCENTER, finalDC);
}

void Engine (Manager *manager)
{
    assert (manager);                                                       
	for (;;)
	{
        txClearConsole();
        if (testMode) printf ("FPS: %lf\n", txGetFPS());
		txSetAllColors (BackgroundColor);
		txRectangle (0, 0, 2000, 2000);

        Vector mp = {txMouseX (), txMouseY ()};
        manager->mousePos = mp;
		manager->draw ();
		if (manager->finalDC) txBitBlt (manager->rect.pos.x, manager->rect.pos.x, manager->finalDC);
		if (txMouseButtons () && manager->rect.inRect (txMouseX (), txMouseY ()))
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



void RECTangle (const Rect rect, HDC dc /* = txDc ()*/)
{
    assert (dc);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}

void LaysMenu::onClick (Vector mp)
{
    mousePos = mp;
    if (advancedMode)
    {

	    if (advancedMode && !isClicked)
	    {
		    clickHandle ();

		    if (canvasManager->activeWindow != NULL)
		    {
			    for (int i = 0; i < ((Canvas*)canvasManager->activeWindow)->currentHistoryLength; i++)
			    {
				    Rect button = {.pos = {(double)i, handle.rect.getSize().y + i * sectionHeight}, .finishPos = {rect.getSize().x, handle.rect.getSize().y +  (i + 1) * sectionHeight}};
                    if (button.inRect (mp))
                    {
                        ((Canvas*)canvasManager->activeWindow)->activeLayNum = i;
                    }
                }
            }
        }
    }
}

void LaysMenu::draw()
{
	txSetAllColors(color, finalDC);
	txRectangle(0, 0, DCMAXSIZE, DCMAXSIZE, finalDC);
	char text[30] = {};


    rect.finishPos.y = handle.rect.getSize().y + rect.pos.y;
	if (canvasManager->activeWindow != NULL)
	{
        rect.finishPos.y = handle.rect.getSize().y + rect.pos.y + sectionHeight * ((Canvas*)canvasManager->activeWindow)->currentLayersLength; 
        
		for (int i = 0; i < ((Canvas*)canvasManager->activeWindow)->currentLayersLength; i++)
		{

			sprintf(text, "Слой %d", i + 1);

			txSetTextAlign(TA_CENTER, finalDC);
			txSetAllColors(TextColor, finalDC);
            selectFont ("Arial", sectionFont, finalDC);
			
			txDrawText(sideThickness, sideThickness + handle.rect.getSize().y + sectionHeight * i, rect.getSize().x, handle.rect.getSize().y + sectionHeight * (i + 1), text, DT_VCENTER, finalDC);

			txLine(0, handle.rect.getSize().y + sectionHeight * (i), rect.getSize().x, handle.rect.getSize().y + sectionHeight * (i), finalDC);
			
		}
	}
	if (test1)printBlt(finalDC);
	handle.print(finalDC);
	controlHandle();
    //txSetAllColors (TX_WHITE, finalDC);
	//txRectangle(0, 0, SIDETHICKNESS, rect.getSize().y, finalDC);
	//txRectangle(0, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, rect.getSize().y, finalDC);
	//txRectangle(rect.getSize().x - SIDETHICKNESS, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, 0, finalDC);
	if (test1)printBlt(finalDC);

    //txSetAllColors(TX_BLUE, finalDC);
	//txRectangle(0, 0, DCMAXSIZE, DCMAXSIZE, finalDC);
    //printBlt (finalDC);
}

void History::draw ()
{
	txSetAllColors (TX_BLACK, finalDC);
	txRectangle (0, 0, DCMAXSIZE, DCMAXSIZE, finalDC);

	
	

	if (canvasManager->activeCanvas != NULL)
	{
		rect.finishPos.y = rect.pos.y + handle.rect.getSize().y + (toolHDCSize) * canvasManager->activeCanvas->currentHistoryLength;
		for (int i = 0; i < canvasManager->activeCanvas->currentHistoryLength; i++)
		{
		
			if (canvasManager->activeCanvas != NULL)
			{
				int tool = 0;
				if (canvasManager->activeCanvas->historyOfDrawingMode[canvasManager->activeCanvas->currentHistoryNumber - 1 - 1] > 0 && canvasManager->activeCanvas->currentHistoryNumber - 1 - 1 - i >= 0)	
				{
					txBitBlt (finalDC, (toolHDCSize / 4), handle.rect.getSize().y + (toolHDCSize) * i + (toolHDCSize / 4), 0, 0, toolsDC[canvasManager->activeCanvas->historyOfDrawingMode[canvasManager->activeCanvas->currentHistoryNumber - 1 - 1 - i] - 1]);
					tool = canvasManager->activeCanvas->historyOfDrawingMode[canvasManager->activeCanvas->currentHistoryNumber - 1 - 1 - i];
				}
		
				if (canvasManager->activeCanvas->currentHistoryNumber - 1 - 1 - i < 0 && canvasManager->activeCanvas->historyOfDrawingMode[10 +canvasManager->activeCanvas->currentHistoryNumber - 1 - 1 - i]  >  0)
				{
					txBitBlt (finalDC, (toolHDCSize / 4), handle.rect.getSize().y + (toolHDCSize) * i + (toolHDCSize / 4), 0, 0, toolsDC[canvasManager->activeCanvas->historyOfDrawingMode[10 +canvasManager->activeCanvas->currentHistoryNumber - 1 - 1 - i] - 1]);	
					tool = canvasManager->activeCanvas->historyOfDrawingMode[10 +canvasManager->activeCanvas->currentHistoryNumber - 1 - 1 - i];
				}
				//txSetTextAlign (TA_LEFT);
				//char *num = new char[canvasManager->activeCanvas->currentHistoryLength]{};
				char strNum[10] = {};
				sprintf (strNum, "%d", canvasManager->activeCanvas->currentHistoryLength - i);
				

				char toolName[50] = {};
				if (tool == 1)
				{
					sprintf (toolName, "Линия");
				}
				if (tool == 2)
				{
					sprintf (toolName, "Ластик");
				}
				if (tool == 4)
				{
					sprintf (toolName, "Ручка");
				}

				//txTextOut (0, handle.rect.getSize().y + (toolHDCSize) * i, strNum, finalDC);
				
				

				//txTextOut ((toolHDCSize + rect.getSize().x) / 2, handle.rect.getSize().y + toolHDCSize * i + toolHDCSize / 2, toolName, finalDC);
				//printBlt(finalDC);
				txSelectFont ("Arial", 18, 5, FW_DONTCARE, false, false, false, 0, finalDC);
				txSetTextAlign(TA_CENTER, finalDC);
				txSetAllColors(TextColor, finalDC);
				txDrawText (toolHDCSize + rect.getSize().x * 0.05, handle.rect.getSize().y + toolHDCSize * i, rect.getSize().x, handle.rect.getSize().y + toolHDCSize * (i + 1), toolName, DT_VCENTER, finalDC);

				txSetAllColors(MenuColor, finalDC);
				txLine (0, handle.rect.getSize().y + toolHDCSize * (i), rect.getSize().x, handle.rect.getSize().y + toolHDCSize * (i), finalDC);
			}

		}
	}

	handle.print (finalDC);
	controlHandle ();
	
	txRectangle (0, 0, SIDETHICKNESS, rect.getSize().y, finalDC);
	txRectangle (0, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, rect.getSize().y, finalDC);
	txRectangle(rect.getSize().x - SIDETHICKNESS, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, 0, finalDC);
}


void History::onClick (Vector mp)
{
    mousePos = mp;
	int mx = mp.x;
	int my = mp.y;
	if (advancedMode && !isClicked)
	{
		clickHandle ();

		if (canvasManager->activeCanvas != NULL)
		{
			for (int i = 0; i < canvasManager->activeCanvas->currentHistoryLength; i++)
			{
					Rect button = {.pos = {getAbsCoordinats().x + i, getAbsCoordinats().y + handle.rect.getSize().y + i *toolHDCSize}, .finishPos = {getAbsCoordinats().x + rect.getSize().x, getAbsCoordinats().y + handle.rect.getSize().y +  (i + 1) * toolHDCSize}};
					if (button.inRect (txMouseX(), txMouseY()))
					{
						//int savecurrentHistoryNumber - 1 = canvasManager->activeCanvas->currentHistoryNumber - 1;
						//canvasManager->activeCanvas->currentHistoryNumber - 1 -= (i);
						//if (canvasManager->activeCanvas->currentHistoryNumber - 1 < 0) canvasManager->activeCanvas->currentHistoryNumber - 1 += 9;
						canvasManager->activeCanvas->currentHistoryLength -= (i);

						//if (i != 0)canvasManager->activeCanvas->canvas = canvasManager->activeCanvas->history[canvasManager->activeCanvas->currentHistoryNumber - 1];
					}

			}
		}
		
	}

}

void TimeButton::draw ()
{
	if (manager)txSetAllColors (manager->color, finalDC);
	txRectangle(0, 0, getSize().x, getSize().y, finalDC);
	time_t t = time (NULL);
	t = t % (24 * 3600);

	char newStr[50] = {};
	int hours =	t / 3600;
	int minutes = t / 60 - hours * 60;
	int second  = t - hours * 3600 - minutes * 60;

	sprintf (newStr, "%d:%02d:%02d", hours + 3, minutes, second);

	txSetTextAlign (TA_LEFT, finalDC);
	txSetAllColors (color, finalDC);
	txSelectFontDC ("Arial", font, finalDC);
	txTextOut (0, 0, newStr, finalDC);
	//printBlt(finalDC);

}



void SizeButton::draw ()
{
	$s
	if (dc) txBitBlt (rect.pos.x, rect.pos.y, dc);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 5, 
					text);
}

void CleanButton::draw()
{
	$s

	if (dc) copyOnDC (0, 0, dc);
		

	/*
	txSetTextAlign (TA_CENTER);
	txSetAllColors (TX_BLACK);  */
	
	/*
	txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 4, 
					text, finalDC);
					*/
}

void CloseButton::draw()
{
	$s
	//txSetAllColors (color);
	//txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	txBitBlt (finalDC, 0, 0, 0, 0, dc);
}



Canvas* CanvasManager::getActiveCanvas()
{
    return activeCanvas;
}


void CanvasManager::draw ()
{
    //txSetAllColors (TX_GREEN, finalDC);
   ///txRectangle (0, 0, 3000, 3000, finalDC);
    //return;
   
	txSetAllColors (BackgroundColor, finalDC);
	txRectangle (0, 0, 3000, 3000, finalDC);

    controlMouse ();

	//if (activeCanvas)bar->setProgress(&activeCanvas->filter->totalProgress, &activeCanvas->filter->currentProgress);
	if (addNewCanvas)
	{
		addWindow(new Canvas({ .pos = {100, 50}, .finishPos = {newCanvasWindowSize.x + 100, newCanvasWindowSize.y + 50} }, closeCanvasButton));
        addNewCanvas = false;
	}

    //if (key (VK_SPACE)) _getch();
    standartManagerDraw (this);
    activeWindow = activeCanvas;
    //printf ("%p\n", activeWindow);


    /*
	for (int i = 0; i < newButtonNum; i++)
	{
		pointers[i]->draw ();

 		if (pointers[i]->advancedMode) txBitBlt (finalDC, pointers[i]->rect.pos.x, pointers[i]->rect.pos.y, pointers[i]->rect.getSize().x, pointers[i]->rect.getSize().y, pointers[i]->finalDC); 

		if (clicked != 1)
		{
			pointers[i]->isClicked = false;
		}
	}  */

	if (manager->getActiveWindow () != this && manager) 
	{
		//activeWindow = NULL;
	}
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
    
    //printf ("%d: %p\n", clickedCellNum, activeCanvas);
    //activeWindow = pointers[clickedCellNum];

    /*
	if (advancedMode)
	{


		for (int i = newButtonNum - 1; i >= 0; i--)
		{
			if (pointers[i]->
           ().inRect(mx, my))
			{
				activeWindow = pointers[i];
				activeCanvas = (Canvas*)pointers[i];
				//bar->setProgress(&activeCanvas->filter->totalProgress, &activeCanvas->filter->currentProgress);
				pointers[i]->onClick();
				pointers[i]->isClicked = true;

				missClicked = false;

				if (pointers[i]->advancedMode) 
				{
					replaceWindow (i);
					break;
				}
			}
			else
			{
				pointers[i]->isClicked = false;

				missClicked = true;
			}
		}
	}

	if (missClicked == true) activeWindow = NULL;
    */
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
	if(manager) txSetAllColors(manager->color, finalDC);
	txRectangle (0, 0, rect.getSize().x, rect.getSize().y, finalDC);

	txSetAllColors(color, finalDC);
	if (totalNum && currentNum)
	{
		txRectangle(0, 0, rect.getSize().x * (*currentNum / *totalNum), rect.getSize().y, finalDC);
	}
	//printBlt(finalDC);
} 

void Menu::draw()
{
    if (dc) txBitBlt(finalDC, 0, 0, 0, 0, dc);

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
    



    if (advancedMode && !isClicked)
    {
        reDraw = true;
        missClicked = onClickLine(mp);
    }

    if (missClicked == true) activeWindow = NULL;
}


void Canvas::draw ()
{
    controlMouse();
    txSetFillColor(TX_BLACK, finalDC);
    txRectangle(0, 0, 3000, 3000, finalDC);
	txSetAllColors (BackgroundColor, finalDC); 

    currentDate->mousePos = mousePos;
    currentDate->managerPos = getAbsCoordinats();
    currentDate->color = DrawColor;
    currentDate->canvasCoordinats = canvasCoordinats;
    currentDate->gummiThickness = GummiThickness;
    currentDate->backGroundColor = TRANSPARENTCOLOR;

    if (testMode) printf("Clicked: %d\n", clicked);

	//controlFilter();

    cleanOutputLay();                                                                                                                      
    controlLay();

    if (activeTool) controlTool();

    drawLay();


	if (txGetAsyncKeyState('Q'))
	{
		endtillkey('Q');
		//playHistory ();
		createLay();

	}

    if (txGetAsyncKeyState ('2'))
    {
        DrawingMode = 2;
    }

	
	if (manager->activeWindow != this) wasClicked = false;

	 
	scrollBarVert.draw ();
	txBitBlt (finalDC, scrollBarVert.rect.pos.x, scrollBarVert.rect.pos.y, scrollBarVert.rect.getSize().x, scrollBarVert.rect.getSize().y, scrollBarVert.finalDC);
	scrollBarHor.draw ();
	txBitBlt (finalDC, scrollBarHor.rect.pos.x, scrollBarHor.rect.pos.y, scrollBarHor.rect.getSize().x, scrollBarHor.rect.getSize().y, scrollBarHor.finalDC);

	controlHandle();
	drawOnFinalDC(handle);
	
	controlSize();

	closeCanvas.rect.pos = {rect.getSize().x - MENUBUTTONSWIDTH,  0};
	closeCanvas.rect.finishPos = {rect.getSize().x , HANDLEHEIGHT};
	
	
	drawOnFinalDC(closeCanvas);

	
}



CLay* Canvas::getActiveLay()
{
    if (activeLayNum < 0) assert(!"Lay hasn't created");
    return &(lay[activeLayNum]);
} 

int Canvas::getCurrentToolLengthOnActiveLay()
{
    return getActiveLay()->getCurrentSize();
}


void Canvas::controlTool()
{
    CLay* clay = getActiveLay();
    ToolLay* toollay = clay->getActiveToolLay();
    Tool* tool = toollay->getTool();

    if (toollay->useTool (currentDate))
    {
        finishTool();
    }
    if (clicked == 0) tool->setMBCondition (clicked);
}

void Canvas::finishTool()
{
    activeTool = false;
    //txTransparentBlt(lay[activeLayNum].outputLay, 0, 0, 0, 0, lay[activeLayNum].lay, 0, 0, TRANSPARENTCOLOR);
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
	//if (txGetAsyncKeyState(VK_LEFT)) SecondFilterValue++;
	//if (txGetAsyncKeyState(VK_RIGHT)) SecondFilterValue--;
	//if (txGetAsyncKeyState(VK_DOWN)) FirstFilterValue+=10;
	//if (txGetAsyncKeyState(VK_UP)) FirstFilterValue-=10;
    /*
	filter->algorithm = FilterAlgorithm;


	if (manager->activeWindow != this) return;
	if (confirmFilter)
	{
		confirmFilter = false;
		nonConfirmFilter = false;
		txBitBlt(canvas, 0, 0, 0, 0, tempFilterDC);
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
	txSetAllColors(MenuColor, finalDC);
	txRectangle (0, 0, SIDETHICKNESS, rect.getSize().y, finalDC);
	txRectangle (0, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, rect.getSize().y, finalDC);
	txRectangle(rect.getSize().x - SIDETHICKNESS, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, 0, finalDC);
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
	if (clicked != 1) isResizing = false;
}

void Canvas::controlSizeSliders ()
{
	//scrollBarVert.resize ({.pos = {rect.finishPos}});
}



/*
void Filter::reStartCount()
{
	lastX = 0;
	lastY = 0;
} */ 
 

void SizeButton::onClick (Vector mp)
{
	if (sizeType > 0)
	{
		(*num)++;
	}
	if (sizeType < 0 && num > 0)
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
	//txBitBlt (history[currentHistoryNumber - 1], 0, 0, 0, 0, canvas);
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
		printBlt(lastSavedDC);
	}  */
	
	//history[addNewHistoryNum].toolsNum = DrawingMode;
	//history[addNewHistoryNum].pos = {lastClick.x + canvasCoordinats.x,  lastClick.y + canvasCoordinats.y};
	//history[addNewHistoryNum].size = size;
	//history[addNewHistoryNum].color = DrawColor;
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
    ToolLay* activeToolLay = getActiveLay()->getActiveToolLay();
    if (txGetAsyncKeyState('E') && currentToolLength > 0)
    {
        while (txGetAsyncKeyState('E')) {};
        editingMode = !editingMode;
        activeToolLay->needRedraw();
    }

    if (editingMode)
    {
        activeToolLay->getTool()->setMBCondition(clicked);
        activeToolLay->editTool(currentDate);
    }
}

bool Canvas::onClickEditLay()
{
    if (editingMode)
    {
        
        return true;
    } 
    return false;
}

bool Canvas::controlLay ()
{
    ToolLay* activeToolLay = getActiveLay()->getActiveToolLay();
    if (txGetAsyncKeyState(VK_RIGHT) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {1, 0};
        activeToolLay->needRedraw();
    }
    if (txGetAsyncKeyState(VK_LEFT) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {-1, 0};
        activeToolLay->needRedraw();
    }
    if (txGetAsyncKeyState(VK_DOWN) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {0, 1};
        activeToolLay->needRedraw();
    }
    if (txGetAsyncKeyState(VK_UP) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {0, -1};
        activeToolLay->needRedraw();
    }

    if (txGetAsyncKeyState('T') && currentToolLength > 0)
    {
        activeToolLay->size += { 0.01, 0};
        activeToolLay->needRedraw();
    }
    if (txGetAsyncKeyState('G') && currentToolLength > 0)
    {
        activeToolLay->size += {-0.01, 0};
        activeToolLay->needRedraw();
    }
    if (txGetAsyncKeyState('Y') && currentToolLength > 0)
    {
        activeToolLay->size += { 0, 0.01};
        activeToolLay->needRedraw();
    }
    if (txGetAsyncKeyState('H') && currentToolLength > 0)
    {
        activeToolLay->size += { 0, -0.01};
        activeToolLay->needRedraw();
    }

    controlEditLay();

	return false;
}

void Canvas::cleanOutputLay()
{
    for (int i = 0; i < currentLayersLength; i++)
    {
        //if (lay[i].redrawStatus ()) txBitBlt(lay[i].getDCForToolLoad(), 0, 0, 0, 0, lay[i].getPermanentDC());
        //txAlphaBlend(lay[i].outputLay, 0, 0, 0, 0, lay[i].lay);
        //lay[i].clean(lay[i].outputLay);
        //txTransparentBlt (lay[i].outputLay, 0, 0, 0, 0, lay[i].lay, 0, 0, TRANSPARENTCOLOR);
        //txBitBlt(0, 0, lay[i].tempLay);
        //while (txGetAsyncKeyState('G')) { txSleep(0); }
        
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
            txTransparentBlt(lay[lays].lay.outputLay, lay[lays].lay.layCoordinats.x, lay[lays].lay.layCoordinats.y, 0, 0, lay[lays].lay.lay, 0, 0, TRANSPARENTCOLOR);
        }
        txTransparentBlt(finalDC, lay[lays].lay.layCoordinats.x, lay[lays].lay.layCoordinats.y, 0, 0, lay[lays].lay.outputLay, 0, 0, TRANSPARENTCOLOR);
    }
    
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



/*
void Lay::line (int x0, int y0, int x1, int y1, double t)
{
	double delta = 0.1;

	//double k = (y1 - y0) / (x1 - x0);

	for (double t = 0; t <= 1; t += 0.00001)
	{
		int x = x0 + t * (x1 - x0);
		int y = y0 + t * (y1 - y0);

		RGBQUAD* color = &layBuf[(int) (x + (DCMAXSIZE - y) * DCMAXSIZE)];
		color->rgbRed = txExtractColor (DrawColor, TX_RED);
		color->rgbGreen = txExtractColor (DrawColor, TX_GREEN);
		color->rgbBlue = txExtractColor (DrawColor, TX_BLUE);
		color->rgbReserved = 255;

		//txSetPixel (x, y, DrawColor, lay);
		//txSetAllColors (DrawColor, lay);
		//txEllipse (x- 3, y-3, x + 3, y + 3, lay);
	}

}
*/


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
    lay[currentLayersLength].createLay();
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
	HDC historyDC = txCreateCompatibleDC (canvasSize.x, canvasSize.y);

	if (stepBack <= currentHistoryLength && stepBack < HistoryLength)
	{
		txBitBlt(historyDC, 0, 0, 0, 0, lastSavedDC);
		//printBlt(lastSavedDC);

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
	if (dc) txDeleteDC (dc);
	if (finalDC) txDeleteDC (finalDC);
	if (canvas) txDeleteDC (canvas);
 	closeCanvas.deleteButton ();
	scrollBarVert.deleteButton ();
	scrollBarHor.deleteButton ();
	deleteHistory();
	
}

void Canvas::startTool()
{
    /*
    saveHistory();
    history[currentHistoryNumber - 1].tools = ToolManager.tools[DrawingMode - 1];
    history[currentHistoryNumber - 1].tools->clicked = clicked;
    history[currentHistoryNumber - 1].toolsData = new char[ToolManager.tools[DrawingMode - 1]->ToolSaveLen];
    currentDate->size = { lineThickness, lineThickness };
    activeTool = true;
    */

    initToolLay();

    controlTool();
}

void Canvas::initToolLay()
{
    
    currentDate->size = { lineThickness, lineThickness };
    addToolLay();
    getActiveLay()->addTool(&toolLays[currentToolLength]);

    currentToolLength++;
}

void Canvas::addToolLay()
{
    assert(LayersNum >= currentToolLength);
    activeTool = true;
    getNewToolLay()->tool = ToolManager.tools[DrawingMode - 1];
    getNewToolLay()->tool->clicked = clicked;
    getNewToolLay()->toolsData = new char[ToolManager.tools[DrawingMode - 1]->ToolSaveLen];
}

ToolLay* Canvas::getNewToolLay()
{
    return &(toolLays[currentToolLength]);
}

void Canvas::onClick(Vector mp)
{
    if (clicked == 1)
    {

        txSetAllColors(drawColor);
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

        if (!(isClicked) && manager->activeWindow == this)
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

        if (!activeTool && !editingMode)
        {
            startTool();
        }


    }

    if (editingMode)
    {
        if (getActiveLay()->getActiveToolLay()->isInToolZone(currentDate, mousePos, clicked)) return;
    }

    
    //independet scenery block++++++++++++++++++++++++++++++++++++++++++++++++++++
    getActiveLay()->getActiveToolLay()->getTool()->setMBCondition(clicked);
    if (activeTool) controlTool();
    //independet scenery block----------------------------------------------------
    
}

void ColorButton::onClick (Vector mp)
{
	DrawColor = color;
}



void CleanButton::onClick (Vector mp)
{
	mainCanvas->clearBackground = true;
}



void invertDC(RGBQUAD* buf, unsigned int totalsize)
{
    for (int i = 0; i < totalsize; i++)
    {
        buf[i].rgbReserved = 255 - buf[i].rgbReserved;
    }
}
