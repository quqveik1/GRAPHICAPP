
#define _CRT_SECURE_NO_WARNINGS
#include "GlobalOptions.h"
#include "..\Macroses.h"
#include "..\Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"
#include <cmath>
#include "C:\Users\���������\Desktop\AlexProjects\Brightness\dllmain.h"
#include "..\Brightness\Q_Filter.h"


HDC TestPhoto;
void compressImage(HDC& newDC, Vector newSize, HDC oldDC, Vector oldSize, int line = NULL);

//#define compressImage(newDC, newSize, oldDC, oldSize)	\
{														\
	_compressImage(newDC, newSize, oldDC, oldSize, NULL);		\
}														\

/*
#define compressImage(newDC, newSize, oldDC, oldSize)  									   \
{																					 	 \
	_compressImage(newDC, newSize, oldDC, oldSize);	 					 \
}																					 	 \
   */
void clearDC(HDC dc, COLORREF color = TX_BLACK);
void bitBlt(RGBQUAD* dest, int x, int y, int sizeX, int sizeY, RGBQUAD* source, int originalSizeX = DCMAXSIZE, int originalSizeY = DCMAXSIZE, int sourceSizeX = DCMAXSIZE, int sourceSizeY = DCMAXSIZE);
void compressDraw(HDC finalDC, Vector pos, Vector finalSize, HDC dc, Vector originalSize, int line = NULL);
void txSetAllColors(COLORREF color, HDC dc = txDC(), int thickness = 1);
void txSelectFontDC(const char* text, int size, HDC& dc);
void copyAndDelete (HDC dest, HDC source);


struct Image
{
	const char* path = NULL;
	HDC dc = NULL;
};

struct CLoadManager
{
	int currentImagesAmount = 0;

	Image images[1000];


	HDC loadImage (const char* path);

};

CLoadManager LoadManager;



struct ProgrammeDate
{
    Vector absMouseCoordinats;
    Vector managerPos;
    Vector canvasCoordinats = {};
    COLORREF color;
    COLORREF backGroundColor;
    int thickness;
    int gummiThickness;
    Vector size;

    ProgrammeDate (Vector _absMouseCoordinats, Vector _managerPos, Vector _size, COLORREF _color) :
        absMouseCoordinats (_absMouseCoordinats),
        managerPos (_managerPos),
        size (_size),
        color (_color)
    {}

};


struct ToolData
{
    int byteLength = 0;

    virtual int getByteSize ();
};

struct ToolSave : ToolData
{
	Vector pos = {};
	Vector size = {};
	COLORREF color = NULL;
	int thickness = NULL;  
    const char *name;

	ToolSave (Vector _pos, Vector _size, COLORREF _color, int _thickness, const char *_name) :
		pos (_pos),
		size (_size),
		color (_color),
		thickness (_thickness),
        name (_name)
	{}

    ToolSave () :
        name (NULL)
    {}

};

struct PointSave : ToolData 
{
    int Length = 100;
    int currentLength = 0;
    ToolSave *points = new ToolSave [Length]{};


    PointSave (int _Length = 100) :
        Length (_Length)
    {
    }

    void addPoint (ToolSave &point);

    virtual int getByteSize();
};


struct ColorSave : ToolData
{
    COLORREF color;
    ColorSave (COLORREF _color = DrawColor) :
        color (_color)
    {}

};

struct Tool
{
    HDC dc;

	const char* name;

    Vector startPos = {};
    bool workedLastTime = false;
    const int ToolSaveLen;

	Tool (const char* _name, const int _ToolSaveLen, HDC _dc = NULL) :
		name (_name), 
        dc (_dc),
        ToolSaveLen (_ToolSaveLen)
	{}


    bool firstUse (ProgrammeDate *data, void* output, Vector currentPos);
    void finishUse ();

    virtual bool use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC);
    virtual void load(void* output, HDC finalDC);
};



struct Line : Tool
{
   
	Line(const char* _name, const int _ToolSaveLen, HDC _dc) :
		Tool(_name, _ToolSaveLen, _dc)
	{
	}


	virtual bool use(ProgrammeDate *data, HDC canvas, void* output, HDC tempDC);
	virtual void load(void* input, HDC finalDC);
};

struct Point : Tool 
{
    Vector lastPos = {};
    PointSave *pointSave = NULL;

    Point(const char* _name, const int _ToolSaveLen, HDC _dc) :
		Tool(_name, _ToolSaveLen, _dc)
	{
	}


    virtual bool use(ProgrammeDate *data, HDC canvas, void* output, HDC tempDC);
    virtual void load(void* input, HDC finalDC);
};

struct Vignette : Tool
{
    Vignette(const char* _name, const int _ToolSaveLen, HDC _dc) :
		Tool(_name, _ToolSaveLen, _dc)
	{
	}

    virtual bool use(ProgrammeDate *data, HDC canvas, void* output, HDC tempDC);
	virtual void load(void* input, HDC finalDC);
};

struct Gummi : Tool 
{

    Gummi (const char* _name, const int _ToolSaveLen, HDC _dc) :
        Tool(_name, _ToolSaveLen, _dc)
    {
    }
    virtual bool use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC);
};

struct RectangleTool : Tool 
{
    RectangleTool (const char* _name, const int _ToolSaveLen, HDC _dc) :
        Tool(_name, _ToolSaveLen, _dc)
    {
    }

    virtual bool use(ProgrammeDate *data, HDC canvas, void* output, HDC tempDC);
	virtual void load(void* input, HDC finalDC);
};

struct CToolManager 
{
    const int ToolsLength = 10;
    Tool **tools = new Tool* [ToolsLength];
    int currentLength = 0;

    void addTool (Tool *tool);
};

CToolManager ToolManager;


struct CHistoryStep
{
	int toolsNum = 0;
	Tool* tools;
    void* toolsData = NULL;
	int thickness = 1;
};





struct Manager;
struct Slider;

struct Lay
{
	HDC lay = {};
	Vector layCoordinats = {}; 
	RGBQUAD *layBuf = {};
	bool isClicked = false;

	void createLay ();
	//void line (int x0, int y0, int x1, int y1, double t = 0.001);
	void line (int x0, int y0, int x1, int y1, COLORREF drawColor = DrawColor);
	void circle (int x, int y, int r);
};

struct Window
{
	Rect rect;
	Rect originalRect;
	COLORREF color;
	const char *text;
	bool isClicked;
	HDC dc;

	HDC finalDC;
	RGBQUAD *finalDCArr;
	Vector finalDCSize; 
	Manager *manager;
	bool advancedMode;
	bool reDraw;

    Window (Rect _rect = {}, COLORREF _color = MenuColor, HDC _dc = NULL, Manager *_manager = NULL, const char *_text = "", bool _advancedMode = true) :
		rect (_rect),
		color(_color),
		manager (_manager),
		text (_text), 
		isClicked (false), 
		dc (_dc),
		advancedMode (_advancedMode),
		reDraw (true)
	{
		if (_rect.getSize().x > 0 && _rect.getSize().y > 0)
		{
			finalDCSize = {_rect.getSize().x, _rect.getSize().y};
			finalDC = txCreateDIBSection(finalDCSize.x, finalDCSize.y, &finalDCArr);
			txSetAllColors(color, finalDC);
			txRectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);
			if (test1) printBlt(finalDC);
		}

		originalRect = rect;
	} 


	Vector getAbsCoordinats (bool coordinatsWithHandle = false);
    Vector getRelativeMousePos (bool coordinatsWithHandle = false);
	Rect getAbsRect (bool coordinatsWithHandle = false);
	
	void resize (Rect newSize);
	void resize (Vector newSize, Vector oldSize);
	void setStartRect (Vector pos, Vector finishPos);
	void print (HDC finalDC);

	Vector getSize();
	virtual void draw ();
	virtual void onClick () {};

	virtual void deleteButton ();
};

struct Manager : Window
{
	int length;
	Window **pointers = NULL;
	int newButtonNum;
	Window *activeWindow;
	Window handle;
	Vector startCursorPos;
	bool coordinatSysFromHandle;

	Manager (Rect _rect,  int _length, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}, COLORREF _color = MenuColor, bool _coordinatSysFromHandle = false) :
		Window (_rect, _color, _dc, NULL, "", _advancedMode),
		length (_length),
		pointers (new Window*[length]{}),
		newButtonNum (0),
		activeWindow (NULL), 
		handle (_handle),
		startCursorPos({}),
		coordinatSysFromHandle (_coordinatSysFromHandle)
	{
		handle.manager = this;
		handle.rect.finishPos.x = DCMAXSIZE * 10;
		handle.color = MenuColor;
	}

	bool addWindow (Window *window);
	Window *getActiveWindow ();
	void controlHandle ();
	void clickHandle ();
	void replaceWindow (int numOfWindow);
	void controlSize ();

	virtual void draw () override;
	virtual void onClick () override;

	virtual void deleteButton () override;
};

struct ToolsPalette : Manager
{
	int lastSelected = 0;
    
	ToolsPalette (Rect _rect, int _length) :
		Manager(_rect, _length, true, NULL, { .pos = {0, 0}, .finishPos = {DCMAXSIZE, HANDLEHEIGHT} }, MenuColor, true)
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
        }
        
	}

	virtual void onClick() override;
	virtual void draw() override;
};
		

struct Slider : Manager 
{
	Window arrow1;
	Window arrow2;
	Window sliderQuadrate;
	double *num;
	double minNum;
	double maxNum;
	double kScale;
	bool horizontalMode;
	double axis;
	Vector cursorStartPosition;
	int tempNum;
	double quadrateScale;
	bool isWhite;

	Slider (Rect _mainRect, double *_num, double _quadrateScale, int _minNum = 0, int _maxNum = 10, bool _horizontalMode = true, bool _isWhite = false) :
		Manager (_mainRect, TX_BLACK),
		arrow1 ({}, TX_RED),
		arrow2 ({}, TX_RED),
		num (_num),
		minNum (_minNum),
		maxNum (_maxNum),
		horizontalMode (_horizontalMode),
		cursorStartPosition ({}),
		sliderQuadrate ({}, TX_WHITE),
		tempNum (0),
		quadrateScale (_quadrateScale),
		isWhite (_isWhite)
	{
		if (isWhite)
		{
			color = TX_WHITE;
		}
		arrow1.manager = this;
		arrow2.manager = this;
		sliderQuadrate.manager = this;

		if (horizontalMode)
		{
			arrow1.rect.pos.x = 0;
			arrow1.rect.pos.y = 0;
			arrow1.rect.finishPos.x = (1) * (rect.getSize ().y);
			arrow1.rect.finishPos.y = rect.getSize ().y;
			arrow1.originalRect = arrow1.rect;
			//compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, LoadManager.loadImage ("arrow1Hor.bmp"), {13, 17});
			compressImage (arrow1.dc, { arrow1.rect.getSize().x, arrow1.rect.getSize().y }, LoadManager.loadImage("arrow1Hor.bmp"), {13, 17}, __LINE__);

			if (isWhite) 
			{
				txDeleteDC (arrow1.dc);
				compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, LoadManager.loadImage ("arrow1WhiteHor.bmp"), {16, 16}, __LINE__);
			}


			arrow2.rect.pos.x = rect.getSize ().x - (1) * rect.getSize ().y;
			arrow2.rect.pos.y = 0;
			arrow2.rect.finishPos.x = rect.getSize ().x;
			arrow2.rect.finishPos.y = rect.getSize ().y;
			arrow2.originalRect = arrow2.rect;
			compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, LoadManager.loadImage ("arrow2Hor.bmp"), {13, 17}, __LINE__);
			if (isWhite) 
			{
				txDeleteDC (arrow2.dc);
				compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, LoadManager.loadImage ("arrow2WhiteHor.bmp"), {16, 16}, __LINE__);
			}
		}
		else
		{
			arrow1.rect.pos.x = 0;
			arrow1.rect.pos.y = 0;
			arrow1.rect.finishPos.x = rect.getSize().x;
			arrow1.rect.finishPos.y = (1) * arrow1.rect.getSize().x;
			arrow1.originalRect = arrow1.rect;
			if (!isWhite) compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, LoadManager.loadImage ("arrow1.bmp"), {17, 13}, __LINE__);
			if (isWhite) 
			{
				compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, LoadManager.loadImage ("arrow1WhiteVert.bmp"), {16, 16}, __LINE__);
			}

			arrow2.rect.pos.x = 0;
			arrow2.rect.pos.y = rect.getSize().y - (1) * arrow1.rect.getSize().x;
			arrow2.rect.finishPos.x = rect.getSize().x;
			arrow2.rect.finishPos.y = rect.getSize().y;
			arrow2.originalRect = arrow2.rect;
			if (!isWhite) compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, LoadManager.loadImage ("arrow2.bmp"), {17, 13}, __LINE__);
			if (isWhite) 
			{
				compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, LoadManager.loadImage ("arrow2WhiteVert.bmp"), {16, 16}, __LINE__);
			}
		}



		if (horizontalMode) 
		{
			if (!isWhite)compressImage (dc, {rect.getSize ().x - arrow1.rect.getSize().x - arrow2.rect.getSize().x, rect.getSize ().y}, LoadManager.loadImage ("SliderBackGroundHor.bmp"), {948, 13}, __LINE__);	
			if (isWhite)compressImage (dc, {rect.getSize ().x - arrow1.rect.getSize().x - arrow2.rect.getSize().x, rect.getSize ().y}, LoadManager.loadImage ("SliderBackGroundWhiteHor.bmp"), {870, 16}, __LINE__);	
			//printBlt(dc);	 	 SliderBackGroundWhiteHot
		}
		else
		{
			if (!isWhite) compressImage (dc, {rect.getSize ().x, rect.getSize ().y - arrow1.rect.getSize().y - arrow2.rect.getSize().y}, LoadManager.loadImage ("SliderBackGroundVert.bmp"), {13, 948}, __LINE__);
			if (isWhite)  compressImage (dc, {rect.getSize ().x, rect.getSize ().y - arrow1.rect.getSize().y - arrow2.rect.getSize().y}, LoadManager.loadImage ("SliderBackGroundWhite.bmp"), {16, 870}, __LINE__);
		}

		if (horizontalMode)
		{
			sliderQuadrate.rect.pos.y       = 0;
			sliderQuadrate.rect.finishPos.y = rect.getSize ().y; 
			sliderQuadrate.rect.pos.x       = arrow1.rect.pos.x;
			sliderQuadrate.rect.finishPos.x = (rect.getSize ().x - arrow1.rect.getSize().x - arrow2.rect.getSize().x) * quadrateScale;
		}
		else
		{
			sliderQuadrate.rect.pos.x       = 1;
			sliderQuadrate.rect.finishPos.x = rect.getSize().x - 1;
			sliderQuadrate.rect.pos.y       = arrow1.rect.pos.y;
			sliderQuadrate.rect.finishPos.y = (rect.getSize ().y - arrow1.rect.getSize().y - arrow2.rect.getSize().y) * quadrateScale; 
		}

		if (horizontalMode)
		{
			axis = (rect.getSize().x - arrow2.rect.getSize ().x) - (arrow1.rect.getSize ().x) - rect.getSize().x * quadrateScale;
		}
		else
		{
			axis = (rect.getSize().y - arrow2.rect.getSize().y) - (arrow1.rect.getSize().y) - rect.getSize().y * quadrateScale;
		}




		if (horizontalMode) 
		{
			if (!isWhite)compressImage (sliderQuadrate.dc, { ceil (rect.getSize ().x * _quadrateScale), sliderQuadrate.rect.getSize().y}, LoadManager.loadImage ("SliderRect.bmp"), {316, 12}, __LINE__);
			if (isWhite)compressImage (sliderQuadrate.dc, { ceil (rect.getSize ().x * _quadrateScale), sliderQuadrate.rect.getSize().y}, LoadManager.loadImage ("WhiteSliderRectHor.bmp"), {299, 1}, __LINE__);
			sliderQuadrate.finalDC = txCreateCompatibleDC (ceil (rect.getSize ().x * _quadrateScale), sliderQuadrate.rect.getSize().y);
			sliderQuadrate.originalRect = {.pos = {0, 0}, .finishPos = {316, 12}};
			//printBlt (sliderQuadrate.dc);
		}
		else
		{

			if (!isWhite) compressImage (sliderQuadrate.dc, {sliderQuadrate.rect.getSize().x, ceil(rect.getSize().y *  _quadrateScale)}, LoadManager.loadImage ("VerticalSliderRect.bmp"), {12, 316}, __LINE__);
			if (isWhite) compressImage (sliderQuadrate.dc, {sliderQuadrate.rect.getSize().x, ceil (rect.getSize().y *  _quadrateScale)}, LoadManager.loadImage ("WhiteSliderRect.bmp"), {14, 299}, __LINE__);
			sliderQuadrate.finalDC = txCreateCompatibleDC (sliderQuadrate.rect.getSize().x, ceil (rect.getSize().y *  _quadrateScale));
			sliderQuadrate.originalRect = {.pos = {0, 0}, .finishPos = {12, 316}};
		}

		arrow1.finalDC = txCreateCompatibleDC (arrow1.rect.getSize().x, arrow1.rect.getSize().y);
		arrow2.finalDC = txCreateCompatibleDC (arrow2.rect.getSize().x, arrow2.rect.getSize().y);
		 

	}

	void resize (Rect newRect);
	void deleteDC ();

	virtual void draw () override;
	virtual void onClick () override;

	virtual void deleteButton() override;

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

struct Canvas : Manager
{
	HDC canvas;
	HDC tempFilterDC;
	RGBQUAD *tempFilterDCArr;
	bool nonConfirmFilter = false; // ���������� ���� �� ������ ������������� ������
	bool reCountEnded = false;
	double lineThickness = 3;
	COLORREF drawColor = TX_RED;
	Window closeCanvas;
	bool clearBackground = true;
	Vector canvasCoordinats;
	Vector canvasSize;
	RGBQUAD *canvasArr;
	//bool confirmFilter;
	Vector startResizingCursor = {};
	bool isResizing = false;
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
	Filter *filter;

	const int LayersNum = 10;
	Lay *lay = new Lay[LayersNum];
	Vector cursorPos = {};

	//HDC *layers = new HDC [LayersNum]{};
	//Vector *layersCoordinats = new Vector [LayersNum]{}; 
	//HDC copyDC = txCreateDIBSection (DCMAXSIZE, DCMAXSIZE);
	//RGBQUAD **layersBuf = new RGBQUAD* [LayersNum]{};
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
		Manager (_rect, 0, true, NULL, {.pos = {0, 0}, .finishPos = {_rect.getSize ().x, HANDLEHEIGHT}}),
		canvasCoordinats ({}),
		canvasSize({DCMAXSIZE, DCMAXSIZE}),
		closeCanvas ({ .pos = {_rect.getSize().x - MENUBUTTONSWIDTH, 0}, .finishPos = {_rect.getSize().x, HANDLEHEIGHT} }, TX_RED, _closeDC, this, "X"),
		scrollBarHor ({.pos = {5, 475}, .finishPos = {475, 495}}, &canvasCoordinats.x, 0.3, 0, 500, true, false),
		scrollBarVert ({.pos = {475, 25}, .finishPos = {495, 475}}, &canvasCoordinats.y, 0.3, 0, 500, false, false)
	{
		canvas = txCreateDIBSection (canvasSize.x, canvasSize.y, &canvasArr);
		scrollBarVert.manager = this;
		scrollBarHor.manager = this;

		tempFilterDC = txCreateDIBSection (canvasSize.x, canvasSize.y, &tempFilterDCArr);

		lastSavedDC = txCreateDIBSection(canvasSize.x, canvasSize.y, &canvasArr);

		filter = new Filter(canvasArr, canvasSize, finalDCArr, tempFilterDCArr, {DCMAXSIZE, DCMAXSIZE}, FilterAlgorithm);
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
	void controlFilter();

	virtual void draw () override;
	virtual void onClick () override;
	virtual void deleteButton () override;
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
	virtual void onClick () override;
};

struct ColorButton : Window
{
	ColorButton(Rect _rect, COLORREF _color, HDC _dc) :
		Window (_rect, _color, _dc)
	{
		draw ();
		reDraw = false;
	}

	virtual void onClick ()override;
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
	virtual void onClick () override;

};

struct CleanButton : Window
{
	Canvas *mainCanvas;

	CleanButton (Rect _rect, COLORREF _color, Canvas *_mainCanvas, HDC _dc = NULL, const char *_text = "") :
		Window (_rect, _color, _dc),
		mainCanvas (_mainCanvas)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};

struct CanvasManager : Manager 
{
	Window newCanvas;
	HDC closeCanvasButton;
	Vector sizeOfNewCanvas;
	Canvas *activeCanvas = NULL; 
	ProgressBar* bar;
	//bool (*reCount)(Filter& filter, int screenSize, bool confirmSecondFilterValue, double SecondFilterValue, double FirstFilterValue);


	CanvasManager (Rect _rect, HDC _NewCanvasDC, ProgressBar* _bar) :
		Manager (_rect, 10, true, NULL, {}, TX_BLACK),
		newCanvas ({.pos = {0, 700}, .finishPos = {50, 750}}, TX_WHITE, _NewCanvasDC), 
		sizeOfNewCanvas ({500, 500}), 
		bar (_bar)
	{
		compressImage (closeCanvasButton, { MENUBUTTONSWIDTH,  HANDLEHEIGHT}, LoadManager.loadImage("CloseButton4.bmp"), {50, 26}, __LINE__);

		
		//color = plus30(color);
		//..FreeLibrary(library);
	}

	virtual void draw () override;
	virtual void onClick() override;

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
		handle.text = "�������";

		for (int i = 0; i < TOOLSNUM; i++)
		{
			compressImage (toolsDC[i], {(double)toolHDCSize/2, (double)toolHDCSize/2}, palette->pointers[i]->dc, {palette->pointers[0]->rect.getSize().y, palette->pointers[0]->rect.getSize().y}, __LINE__);
			 //printBlt (toolsDC[i]);
		}
		
		if (_openCanvas) compressImage (openCanvas, {rect.getSize().x, (double) toolHDCSize}, _openCanvas, {50, 50}, __LINE__);
	}

	virtual void draw () override;
	virtual void onClick () override;
};

struct LaysMenu : Manager
{
	CanvasManager* canvasManager;
	HDC openCanvas;
	int sectionWidth = 20;
	//HDC toolsDC[TOOLSNUM];

	LaysMenu(Rect _rect, CanvasManager* _canvasManager) :
		Manager(_rect, 0, true, NULL, { .pos = {0, 0}, .finishPos = {_rect.getSize().x, HANDLEHEIGHT} }),
		canvasManager(_canvasManager)
	{
		txSelectFont("Arial", 21, 7, FW_DONTCARE, false, false, false, 0, handle.finalDC);
		rect.finishPos.y = rect.pos.y + handle.rect.getSize().y;
		handle.rect.finishPos.x = rect.getSize().x;
		handle.color = TX_WHITE;
		handle.text = "����";

		for (int i = 0; i < TOOLSNUM; i++)
		{
			//compressImage(toolsDC[i], { (double)toolHDCSize / 2, (double)toolHDCSize / 2 }, palette->pointers[i]->dc, { palette->pointers[0]->rect.getSize().y, palette->pointers[0]->rect.getSize().y });
			//printBlt (toolsDC[i]);
		}
	}

	virtual void draw() override;
	//virtual void onClick() override;
};


struct OpenManager : Window
{
	Manager *manager;

	OpenManager (Rect _rect, COLORREF _color, Manager *_manager, HDC _dc = NULL, const char *_text = "")	:
		Window (_rect, _color, _dc),
		manager (_manager)
	{}

	virtual void draw () override;
	virtual void onClick () override;
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
	virtual void onClick () override;
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
	virtual void onClick () override;
};

 
struct ContrastMenu : Manager
{
	Slider upSlider;
	Slider downSlider;
	double brightness = FirstFilterValue;
	double contrast = SecondFilterValue;

	Window confirmButton;
	RGBQUAD(*algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue);

	ContrastMenu(Rect _rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue))	:
		Manager (_rect, 3, true, NULL, { .pos = {0, 0}, .finishPos = {getSize().x, HANDLEHEIGHT} }),
		upSlider({ .pos = {10, 65}, .finishPos = {165, 80}}, &brightness, 0.3, firstDomain.x, firstDomain.y, true, true),
		downSlider({ .pos = {10, 125}, .finishPos = {165, 140} }, &contrast, 0.3, secondDomain.x, secondDomain.y, true, true),
		confirmButton({ .pos = {240, 40}, .finishPos = {325, 60} }),
		algorithm (_algorithm)
	{
		addWindow(&upSlider);
		addWindow(&downSlider);
		addWindow(&confirmButton);
		compressImage(dc, getSize(), LoadManager.loadImage("ContrastMenu.bmp"), {335, 179} );
	}

	virtual void draw() override;
	virtual void onClick() override;
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

bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;



void RECTangle (const Rect rect, HDC dc = txDC ());
void engine (Manager *manager);
void shiftArrBack    (char arr[], int length);
void shiftArrForward (char arr[], int length);
bool checkDeltaTime (int lastTimeClicked);
void printfDCS (const char *str = "");
void createNumChanger (Manager *menu, NumChange **numchange, Canvas *mainCanvas);
void checkTextLen (int currentNum, int previousNum, int *textLen, int *cursorPosition);



int main ()
{
	//_txWindowStyle &= ~WS_CAPTION;
	txCreateWindow (SCREENSIZE.x, SCREENSIZE.y);

    //double x = 10;
    //printf ("%d", sizeof(x));
	/*
	Lay lay;
	lay.createLay ();
	lay.line (0, 0, 1000, 1000);
	lay.line (10, 0, 10, 1000);
	lay.circle (100, 100, 20);
	RGBQUAD *buff;	*/
	//HDC outDC = txCreateDIBSection (DCMAXSIZE, DCMAXSIZE, &buff);
	//bitBlt (buff, 100, 100, DCMAXSIZE, DCMAXSIZE, lay.layBuf);
	//txBitBlt (0, 0, outDC);
	//stop;
	
	//txSelectFont ("Arial", 3, 1);

	//TestPhoto = LoadManager.loadImage ("TestPhoto.bmp");

	//txBitBlt (TestPhoto, 1000, 1000, 0, 0, TestPhoto);

	//compressImage (TestPhoto, {100, 100}, {1200, 727});

	//printBlt (TestPhoto);

	//stop;

	

	Manager *manager = new Manager({.pos = {0, 0}, .finishPos = {SCREENSIZE.x, SCREENSIZE.y}}, 10, true, NULL, {}, TX_RED);

	//printfDCS ();

    ToolSave toolSave = {};
    //PointSave pointSave = {};
    //ColorSave colorSave = {};
    //printf ("getByteSize():%d||sizeof:%d||sizeof(ptr):%d", toolSave.getByteSize(), sizeof (ToolSave), sizeof (*(&toolSave)));
	
	Line *line = new Line ((const char*) (1), sizeof (ToolSave), LoadManager.loadImage ("Line.bmp"));
    ToolManager.addTool (line); 
    Point *point = new Point ((const char*) (2), sizeof (PointSave) + sizeof (ToolSave) * 100, LoadManager.loadImage ("Pen.bmp"));
    ToolManager.addTool (point);
    Vignette *vignette = new Vignette ((const char*) (3), sizeof (ColorSave), LoadManager.loadImage ("Vignette.bmp"));
    ToolManager.addTool (vignette);
    Gummi *gummi = new Gummi ((const char*) (4), sizeof (ToolSave), LoadManager.loadImage ("Gummi.bmp"));
    ToolManager.addTool (gummi);
    RectangleTool *rect = new RectangleTool ((const char*) (1), sizeof (ToolSave), LoadManager.loadImage ("Rectangle.bmp"));
    ToolManager.addTool (rect);


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
	
	//canvasManager->deleteButton();

	//LaysMenu* lays = new LaysMenu({ .pos = {850, 400}, .finishPos = {1000, 944} }, canvasManager);
	//manager->addWindow(lays);

	//printfDCS ();

	
	ToolsPalette *toolsPallete = new ToolsPalette({.pos = {0, 100}, .finishPos = {50, TOOLSNUM * 50 + HANDLEHEIGHT + 100}}, 5);
	manager->addWindow (toolsPallete);
    /*

		 //Window* line = new Window({ .pos = {0, 0}, .finishPos = {50, 50} }, TX_WHITE, LoadManager.loadImage("Line.bmp"));
		//toolsPallete->addWindow(line);

		Window *gummi = new Window ({ .pos = {0, 50}, .finishPos = {50, 100}}, TX_WHITE, LoadManager.loadImage ("Gummi.bmp"));
		toolsPallete->addWindow (gummi); 

		Window *vignette = new Window ({ .pos = {0, 100}, .finishPos = {50, 150} }, TX_WHITE, LoadManager.loadImage ("Vignette.bmp"));
		toolsPallete->addWindow (vignette);

		Window *pen = new Window ({ .pos = {0, 150}, .finishPos = {50, 200}}, TX_WHITE, LoadManager.loadImage("Pen.bmp"));
		toolsPallete->addWindow (pen);

		Window *mover = new Window ({ .pos = {0, 200}, .finishPos = {50, 250}}, TX_WHITE, LoadManager.loadImage("Mover.bmp"));
		toolsPallete->addWindow (mover);

		//printfDCS ();

	History *history = new History ({.pos = {1750, 500}, .finishPos = {1900, 944}}, canvasManager, toolsPallete);
	manager->addWindow (history);
    */

	 
	Manager *menu = new Manager({.pos = {1488, 100}, .finishPos = {1900, 400}}, 3, true, LoadManager.loadImage ("HUD-4.bmp"), {.pos = {0, 0}, .finishPos = {412, 50}});
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
	

	

	Manager* mainhandle = new Manager({ .pos = {0, 0}, .finishPos = {SCREENSIZE.x, HANDLEHEIGHT} }, 3, true, NULL, {}, RGB(45, 45, 45));
	manager->addWindow(mainhandle);

		CloseButton* closeButton = new CloseButton({ .pos = {SCREENSIZE.x - 50, 0}, .finishPos = {SCREENSIZE.x, HANDLEHEIGHT} }, TX_RED, LoadManager.loadImage("CloseButton4.bmp"));
		mainhandle->addWindow(closeButton);
	  
	 
	 /*
	BrightnessButton *brightnessButton = new BrightnessButton ({.pos = {800, 100}, .finishPos = {1244, 461}}, {.pos = {55, 64}, .finishPos = {312, 318}},
															   {.pos = {322, 64}, .finishPos = {338, 318}}, {.pos = {17, 64}, .finishPos = {33, 318}},
															   {.pos = {357, 63}, .finishPos = {436, 80}},  
															   {.pos = {357, 90}, .finishPos = {436, 100}});	
	manager->addWindow(brightnessButton);
	*/
	
	HMODULE library = LoadLibrary("..\\Brightness\\Debug\\Brightness.dll");
	assert(library);
	//algorithm = (RGBQUAD(*)(RGBQUAD pixel, double SecondFilterValue, double FirstFilterValue))GetProcAddress(library, "KontrastFilter");
	//assert(algorithm);
	ContrastMenu* contrastMenu = new ContrastMenu({ .pos = {500, 500}, .finishPos = {835, 679} }, { -10, 10 }, {-256, 256}, (RGBQUAD(*)(RGBQUAD pixel, double SecondFilterValue, double FirstFilterValue))GetProcAddress(library, "KontrastFilter"));
	manager->addWindow(contrastMenu);  
	
	ContrastMenu* brightnessMenu = new ContrastMenu({ .pos = {900, 500}, .finishPos = {1235, 679} }, {-100, 100 }, {-256, 256}, (RGBQUAD(*)(RGBQUAD pixel, double SecondFilterValue, double FirstFilterValue))GetProcAddress(library, "BrightnessKontrastFilter"));
	manager->addWindow(brightnessMenu);
															  
	
	   
	//OpeManager *brightnessButtonOpen = new OpenManager ({.pos = {500, 0}, .finishPos = {550, 50}}, TX_WHITE, brightnessButton);
	//manager->addWindow (brightnessButtonOpen);
	//printBlt (brightnessButtonOpen->finalDC);
	
	
	
	txBegin ();



	engine (manager);

	txEnd ();

	//redColor->deleteButton ();
	//blueColor->deleteButton ();
	//blueColor->deleteButton ();

	manager->deleteButton ();	
	 

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
		sprintf(text, "������: ����������� ������");
	}
	if (currentNum == 0)
	{
		sprintf(text, "������: ������� ����� ���");
	}
	txSetTextAlign(TA_LEFT, finalDC);
	txTextOut(10, 0, text, finalDC);

	txBitBlt(finalDC, timeButton->rect.pos.x, timeButton->rect.pos.y, timeButton->getSize().x, timeButton->getSize().y, timeButton->finalDC);

}

void txSelectFontDC(const char* text, int size, HDC &dc)
{
	txSelectFont(text, size, -1, FW_DONTCARE, false, false, false, 0, dc);
}

void ContrastMenu::onClick()
{
	if (!advancedMode) return;
	int mx = txMouseX();
	int my = txMouseY();

	if (handle.getAbsRect().inRect(mx, my))
	{
		startCursorPos.x = mx;
		startCursorPos.y = my;
		handle.isClicked = true;
	}

	if (upSlider.getAbsRect().inRect(mx, my) && !isClicked)
	{
		upSlider.onClick();
	}
	if (downSlider.getAbsRect().inRect(mx, my) && !isClicked)
	{
		downSlider.onClick();
	}
	//if (closeButton.getAbsRect().inRect(mx, my) && !isClicked) advancedMode = false;
	if (confirmButton.getAbsRect().inRect(mx, my) && !isClicked)
	{
		confirmFilter = true;
		//brightnessSlider.maxNum = Brightness;
		//brightnessSlider.maxNum = 255;
		//FirstFilterValueSlider.maxNum = FirstFilterValue;
	}
}

void ContrastMenu::draw()
{

	if (!advancedMode) return;

	if (txMouseButtons() != 1 && manager->activeWindow == this)
	{
		SecondFilterValue = brightness;
		FirstFilterValue = contrast;
	}

	controlHandle();

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

	//txSetFillColor(TX_RED);
	//txRectangle(300, 300, 400, 400);
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

void BrightnessButton::onClick ()
{
	if (!advancedMode) return;
	int mx = txMouseX ();
	int my = txMouseY ();

	if (handle.getAbsRect().inRect (mx, my))
	{
			startCursorPos.x = mx;
			startCursorPos.y = my;
			handle.isClicked = true;
	}

	if (SecondFilterValueSlider.getAbsRect().inRect (mx, my) && !isClicked) SecondFilterValueSlider.onClick ();		
	if (FirstFilterValueSlider.getAbsRect().inRect (mx, my) && !isClicked) FirstFilterValueSlider.onClick ();		
	if (closeButton.getAbsRect().inRect (mx, my) && !isClicked) advancedMode = false;
	if (confirmButton.getAbsRect().inRect (mx, my) && !isClicked) 
	{
		confirmFilter = true;
		//SecondFilterValueSlider.maxNum = Brightness;
		//brightnessSlider.maxNum = 255;
		//FirstFilterValueSlider.maxNum = FirstFilterValue;
	}
}

void Slider::deleteDC ()
{
	txDeleteDC (sliderQuadrate.dc);
	txDeleteDC (arrow1.dc);
	txDeleteDC (arrow2.dc);
	txDeleteDC (dc);
}

void Slider::resize (Rect newRect)
{
	//arrow1.resize ({.pos = newRect.getSize() * (arrow1.rect.pos / rect.getSize()), .finishPos = newRect.getSize() * (arrow1.rect.finishPos / rect.getSize())});
	//arrow2.resize ({.pos = newRect.getSize() * (arrow2.rect.pos / rect.getSize()), .finishPos = newRect.getSize() * (arrow2.rect.finishPos / rect.getSize())});
	//compressImage (dc, newRect.getSize(), rect.getSize());

	if (horizontalMode)
	{
		arrow1.rect.pos.x = 0;
		arrow1.rect.pos.y = 0;
		arrow1.rect.finishPos.x = (1) * (rect.getSize ().y);
		arrow1.rect.finishPos.y = rect.getSize ().y;

		arrow2.rect.pos.x = rect.getSize ().x - (1) * rect.getSize ().y;
		arrow2.rect.pos.y = 0;
		arrow2.rect.finishPos.x = rect.getSize ().x;
		arrow2.rect.finishPos.y = rect.getSize ().y;
	}
	else
	{
		arrow1.rect.pos.x = 0;
		arrow1.rect.pos.y = 0;
		arrow1.rect.finishPos.x = rect.getSize().x;
		arrow1.rect.finishPos.y = (1) * arrow1.rect.getSize().x;
		
		arrow2.rect.pos.x = 0;
		arrow2.rect.pos.y = rect.getSize().y - (1) * arrow1.rect.getSize().x;
		arrow2.rect.finishPos.x = rect.getSize().x;
		arrow2.rect.finishPos.y = rect.getSize().y;
	}

	rect = newRect;
	if (horizontalMode)
	{
		axis = (rect.getSize().x - arrow2.rect.getSize ().x) - (arrow1.rect.getSize ().x) - rect.getSize().x * quadrateScale;
	}
	else
	{
		axis = (rect.getSize().y - arrow2.rect.getSize ().y) - (arrow1.rect.getSize ().y) - rect.getSize().y * quadrateScale;
	}

	clearDC (arrow1.finalDC);
	clearDC (arrow2.finalDC);
	clearDC (finalDC);
}

void clearDC (HDC dc, COLORREF color /* = TX_BLACK*/)
{
	$s;
	txSetAllColors (color, dc);

	txRectangle (0, 0, DCMAXSIZE, DCMAXSIZE, dc);
}

void compressImage (HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize, int line)
{

	char str[10] = {};
	sprintf(str, "%d", line);
	if (newSize.x <= 0 || newSize.y <= 0)
		newSize.print (str);

	newDC = txCreateCompatibleDC (newSize.x, newSize.y);

	
	//if (test1)printBlt (oldDC);

	assert (oldDC);
	if (!StretchBlt (newDC, 0, 0, newSize.x, newSize.y, oldDC, 0, 0, oldSize.x, oldSize.y, SRCCOPY))
	{
		//printBlt (newDC);
		printBlt (oldDC);
	}

	if (test1)printBlt (newDC);
}

void compressDraw (HDC finalDC, Vector pos, Vector finalSize, HDC dc, Vector originalSize, int line)
{
	HDC copyDC = NULL;
	//if (test1)printBlt (dc);
	compressImage (copyDC, finalSize, dc, originalSize, line);
	//printBlt (dc);
	if (test1)
	{
		//printBlt (copyDC);
		//txRectangle (0, 0, 300, 300)
	}
	txBitBlt (finalDC, pos.x, pos.y, finalSize.x, finalSize.y, copyDC);
	txDeleteDC (copyDC);
}

void Slider::deleteButton ()
{
	arrow1.deleteButton();
	arrow2.deleteButton();
	sliderQuadrate.deleteButton();
	if (dc) txDeleteDC (dc);
	if (finalDC) txDeleteDC (finalDC);
}

void Slider::draw ()
{
	if (maxNum - minNum != 0) kScale = axis / (maxNum - minNum);
	else kScale = 0;

	int mx = txMouseX ();
	int my = txMouseY ();
	Vector mousePos =  {mx, my};

	//txBitBlt (finalDC, arrow2.rect.pos.x, arrow2.rect.pos.y, rect.getSize ().x, rect.getSize ().y, arrow2.finalDC);

	//upSideOfQuadrateSlider.draw ();
	//txBitBlt (finalDC, upSideOfQuadrateSlider.rect.pos.x, upSideOfQuadrateSlider.rect.pos.y, rect.getSize ().x, rect.getSize ().y, upSideOfQuadrateSlider.finalDC);

	//bottomSideOfQuadrateSlider.draw ();
	//txBitBlt (finalDC, bottomSideOfQuadrateSlider.rect.pos.x, bottomSideOfQuadrateSlider.rect.pos.y, rect.getSize ().x, rect.getSize ().y, bottomSideOfQuadrateSlider.finalDC);

	if (txMouseButtons () != 1) sliderQuadrate.isClicked = false;

	
	//copyOnDC (sliderQuadrate.rect.pos.x, sliderQuadrate.rect.pos.y, sliderQuadrate.dc);
	//txBitBlt (finalDC, arrow2.rect.pos.x, arrow2.rect.pos.y, rect.getSize ().x, rect.getSize ().y, sliderQuadrate.dc);


	if (sliderQuadrate.isClicked)
	{
		if (horizontalMode && kScale != 0)  *num = tempNum + ( (mousePos.x - cursorStartPosition.x) / (kScale));
		if (!horizontalMode && kScale != 0) *num = tempNum + ( (mousePos.y - cursorStartPosition.y) / (kScale)); 

		if (*num > maxNum) *num = maxNum;
		if (*num < minNum) *num = minNum;
	}

	if (horizontalMode)
	{
		sliderQuadrate.rect.pos.x		= kScale * (*num - minNum) + arrow1.rect.finishPos.x;
		sliderQuadrate.rect.finishPos.x = kScale * (*num - minNum) + arrow1.rect.finishPos.x + quadrateScale * rect.getSize ().x;
	}
	else
	{
		sliderQuadrate.rect.pos.y		= kScale * (*num - minNum) + arrow1.rect.finishPos.y;
		sliderQuadrate.rect.finishPos.y = quadrateScale * rect.getSize ().y + sliderQuadrate.rect.pos.y;
	}

	txSetAllColors (TX_WHITE, finalDC);

	//if (horizontalMode)compressDraw (finalDC, {arrow1.rect.finishPos.x, arrow1.rect.pos.y }, {arrow2.rect.pos.x - arrow1.rect.finishPos.x, arrow2.rect.finishPos.y - arrow1.rect.pos.y}, dc, originalRect.getSize());
	if (horizontalMode)compressDraw (finalDC, {arrow1.rect.finishPos.x, arrow1.rect.pos.y }, {rect.getSize ().x, rect.getSize ().y}, dc, originalRect.getSize(), __LINE__);
	if (!horizontalMode)compressDraw (finalDC, {arrow1.rect.pos.x, arrow1.rect.finishPos.y }, {rect.getSize ().x, rect.getSize ().y}, dc, originalRect.getSize(), __LINE__);
	//if (horizontalMode) compressDraw (finalDC, {0, 0}, {rect.getSize ().x, rect.getSize ().y}, dc, originalRect.getSize());

	arrow1.draw ();
	compressDraw (finalDC, arrow1.rect.pos, arrow1.rect.getSize (), arrow1.finalDC, arrow1.originalRect.getSize (), __LINE__);
	//printBlt(arrow1.finalDC);
	//printBlt (arrow1.finalDC);
	//txBitBlt (finalDC, arrow1.rect.pos.x, arrow1.rect.pos.y, rect.getSize ().x, rect.getSize ().y, arrow1.finalDC);


	arrow2.draw ();
	compressDraw (finalDC, arrow2.rect.pos, arrow2.rect.getSize (), arrow2.finalDC, arrow2.originalRect.getSize (), __LINE__);
	//printBlt(finalDC);
	//txBitBlt(finalDC, arrow2.rect.pos.x, arrow2.rect.pos.y, arrow2.rect.getSize().x, arrow2.rect.getSize().y, arrow2.finalDC);

	//printBlt (sliderQuadrate.dc);
	sliderQuadrate.draw ();
	txSetAllColors (TX_RED);
	//txRectangle (0, 0, 1000 ,1000);
	//txBitBlt (txDC(), 0, 0, 50, sliderQuadrate.rect.getSize ().y, sliderQuadrate.finalDC );
	
	//txSleep ();
	//stop;
	//printBlt (sliderQuadrate.finalDC)
	//txBitBlt (sliderQuadrate.finalDC, sliderQuadrate.rect.pos.x, sliderQuadrate.rect.pos.y, sliderQuadrate.rect.getSize ().x, sliderQuadrate.rect.getSize ().y);
	//printBlt(finalDC);
	txBitBlt (finalDC, sliderQuadrate.rect.pos.x, sliderQuadrate.rect.pos.y, sliderQuadrate.rect.getSize ().x, sliderQuadrate.rect.getSize ().y, sliderQuadrate.dc);
	//printBlt (finalDC);
}

void Slider::onClick ()
{
	int mx = txMouseX ();
	int my = txMouseY ();
	Vector mousePos =  {mx, my};



	if (sliderQuadrate.getAbsRect().inRect (mx, my) && !isClicked)
	{
		cursorStartPosition = {(double) mx, (double) my};
		tempNum = *num;
		sliderQuadrate.isClicked = true;
	}

	if (arrow1.getAbsRect().inRect (mx, my) && *num > minNum && !isClicked)
	{
		(*num)--;
	}
	if (arrow2.getAbsRect().inRect (mx, my) && *num < maxNum && !isClicked)
	{
		(*num)++;
	}
}


void createNumChanger (Manager *menu, NumChange **numChange, Canvas *mainCanvas)
{

	*numChange = (new NumChange (
								{.pos = {70, 44 - 10}, .finishPos = {389, 74}},
								{.pos = {296, 46}, .finishPos = {362, 74}}, 
								{.pos = {368, 44}, .finishPos = {389, 59}}, 
								{.pos = {368, 59}, .finishPos = {389, 74}}, 
								{.pos = {70, 55}, .finishPos = {270, 65} },
								LoadManager.loadImage ("SwitchButton.bmp"), 
								0.3,
								1, &mainCanvas->lineThickness));
	menu->addWindow (*numChange);
}

void printfDCS (const char *str)
{
    printf ("%s\n", str);
    for (int i = 0; i < _txCanvas_UserDCs->size (); i++)
    {
        printf ("%d: %p\n", i, (*_txCanvas_UserDCs)[i]);
    }
    _getch ();
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
	checkTextLen (*num, numBeforeSlider, &stringButton.textLen, &stringButton.cursorPosition);

	if (txMouseButtons () != 1 && plusNum.isClicked  == true) plusNum.isClicked = false;
	if (txMouseButtons () != 1 && minusNum.isClicked == true) minusNum.isClicked = false;

	if (manager->activeWindow != this) activeWindow = NULL;
}


void NumChange::onClick ()
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
		plusNum.onClick ();
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
		minusNum.onClick ();
		minusNum.isClicked = true;
	}


	if (slider.rect.inRect (mx, my))
	{
		activeWindow = &slider;

		bool changeTextLen = false;
		int numBeforeSlider = *num;

		slider.onClick();

		checkTextLen (*num, numBeforeSlider, &stringButton.textLen, &stringButton.cursorPosition);
	}

}

void checkTextLen (int currentNum, int previousNum, int *textLen, int *cursorPosition)
{
	if (currentNum >= 10)
	{
		*textLen = 3;
	}
	else
	{
		*textLen = 2;
	}

	if (previousNum >= 10 && currentNum <= 9)
	{
		*cursorPosition =  *textLen - 2;
	}
	if (previousNum <= 9 && currentNum >= 10)
	{
		*cursorPosition =  *textLen - 2;
	}
}

Window* Manager::getActiveWindow ()
{
	if (this == NULL) return 0;

	return activeWindow;
}


bool Manager::addWindow (Window *window)
{
	if (newButtonNum >= length) return 0;

	pointers[newButtonNum] = window;
	newButtonNum++;

	window->manager = this;
	
	return 1;
}

void ToolsPalette::draw()
{
	//reDraw = false;

	//controlHandle();
	if (dc) txBitBlt(finalDC, 0, 0, 0, 0, dc);

	controlHandle();
	drawOnFinalDC(handle);

	for (int i = 0; i < newButtonNum; i++)
	{
		pointers[i]->draw();
		//printBlt (pointers[i]->dc);
		if (pointers[i]->advancedMode) txBitBlt(finalDC, pointers[i]->rect.pos.x, pointers[i]->rect.pos.y + handle.rect.finishPos.y, pointers[i]->rect.finishPos.x, pointers[i]->rect.finishPos.y, pointers[i]->finalDC);
		if (lastSelected == i)
		{
			txSetAllColors(TX_WHITE, finalDC);
			txRectangle(pointers[i]->rect.pos.x, pointers[i]->rect.pos.y + handle.rect.finishPos.y, pointers[i]->rect.pos.x + pointers[i]->rect.getSize().x * 0.1, pointers[i]->rect.pos.y + pointers[i]->rect.getSize().y * 0.1  + handle.rect.finishPos.y, finalDC);
		}
		if (txMouseButtons() != 1)
		{
			pointers[i]->isClicked = false;
		}
	}

	if (manager->getActiveWindow() != this && manager)
	{
		activeWindow = NULL;
	}

	
	//handle.print(finalDC);
}

void ToolsPalette::onClick ()
{
	bool missClicked = true;

	int mx = txMouseX();
	int my = txMouseY();
	clickHandle();
	


	if (advancedMode && !isClicked)
	{
		//clickHandle ();

		reDraw = true;
		//clickHandle();

		if (txMouseButtons() == 1)
		{
			//HMODULE library = LoadLibrary("..\\Brightness\\Debug\\Brightness.dll");
			//assert(library);
			//COLORREF (*plus30)(COLORREF color) = (COLORREF (*)(COLORREF color))GetProcAddress(library, "Plus30");
			//assert(plus30);
			//color = plus30(color);
			//..FreeLibrary(library);
		}
		for (int i = newButtonNum - 1; i >= 0; i--)
		{
			if (pointers[i]->getAbsRect().inRect(mx, my))
			{
				activeWindow = pointers[i];
				pointers[i]->onClick();
				pointers[i]->isClicked = true;
				DrawingMode = i + 1;
				lastSelected = i;


				missClicked = false;



				if (pointers[i]->advancedMode) break;
			}
			else
			{
				pointers[i]->isClicked = false;

				missClicked = true;
			}
		}
	}

	if (missClicked == true) activeWindow = NULL;
}

void Manager::deleteButton ()
{
	if (dc) txDeleteDC (dc);
	if (finalDC) txDeleteDC (finalDC);
	for (int i = 0; i < length; i++)
	{
		if (pointers[i]) pointers[i]->deleteButton ();
	}
}

void Manager::draw ()
{
	controlHandle ();


	txSetAllColors (color, finalDC);
	txRectangle (0, 0, DCMAXSIZE, DCMAXSIZE, finalDC);
	if (dc) txBitBlt (finalDC, 0, 0, 0, 0, dc);


	for (int i = 0; i < newButtonNum; i++)
	{
		if (pointers[i]->advancedMode && pointers[i]->reDraw) pointers[i]->draw ();
 		if (pointers[i]->advancedMode) 
		{
			//printBlt(finalDC);
			txBitBlt (finalDC, pointers[i]->rect.pos.x, pointers[i]->rect.pos.y, pointers[i]->rect.getSize().x, pointers[i]->rect.getSize().y, pointers[i]->finalDC);
			//bitBlt (finalDCArr, pointers[i]->rect.pos.x, pointers[i]->rect.pos.y, pointers[i]->rect.getSize().x, pointers[i]->rect.getSize().y, pointers[i]->finalDCArr, pointers[i]->finalDCSize.x, pointers[i]->finalDCSize.y, finalDCSize.x, finalDCSize.y);	
			//printBlt (pointers[i]->finalDC);
		}
		if (txMouseButtons () != 1)
		{
			pointers[i]->isClicked = false;
		}
	}

	if (manager->getActiveWindow () != this && manager) 
	{
		activeWindow = NULL;
	}
}

void Manager::clickHandle ()
{
	if (handle.getAbsRect(true).inRect (txMouseX (), txMouseY ()))
	{
		startCursorPos.x = txMouseX ();
		startCursorPos.y = txMouseY ();
		handle.isClicked = true;
	}
}

void Manager::controlHandle ()
{
	if (handle.isClicked)
	{
		rect.pos.x += txMouseX () - startCursorPos.x;
		rect.pos.y += txMouseY () - startCursorPos.y;
		rect.finishPos.x += txMouseX () - startCursorPos.x;
		rect.finishPos.y += txMouseY () - startCursorPos.y;
		startCursorPos.x = txMouseX (); 
		startCursorPos.y = txMouseY (); 
	}
	if (txMouseButtons () != 1)
	{
		handle.isClicked = false;
	}
	//drawOnFinalDC (handle);
}


/*
void Manager::replaceWindow (int numOfWindow, int startOfWindows)
{
	Window *copyOfStartWindow = pointers[numOfWindow];
	Window *copyOfWindow = pointers[startOfWindows];

	for (int i = startOfWindows; i < numOfWindow; i++)
	{
		Window* preCopyOfWindow = pointers[i + 1];
		pointers[i + 1] = copyOfWindow;
		copyOfWindow = 	preCopyOfWindow;
	}

	 if (startOfWindows < numOfWindow) pointers[startOfWindows] = copyOfStartWindow;

}
*/

void Manager::replaceWindow(int numOfWindow)
{
	Window* copyOfStartWindow = pointers[numOfWindow];
	Window* copyOfWindow = pointers[newButtonNum - 1];

	for (int i = newButtonNum - 1; i > numOfWindow; i--)
	{
		Window* preCopyOfWindow = pointers[i - 1];
		pointers[i - 1] = copyOfWindow;
		copyOfWindow = preCopyOfWindow;
	}

	if (numOfWindow < newButtonNum - 1) 
	{
		pointers[newButtonNum - 1] = copyOfStartWindow;
	}

}


void Manager::onClick ()
{
	bool missClicked = true;

	int mx = txMouseX ();
	int my = txMouseY ();

	if (advancedMode)
	{
		clickHandle();
		if (handle.getAbsRect().inRect (mx, my))
		{
			startCursorPos.x = mx;
			startCursorPos.y = my;
			handle.isClicked = true;
		}
		for (int i = newButtonNum - 1; i >= 0; i--)
		{
			if (pointers[i]->getAbsRect().inRect(mx, my))
			{
				activeWindow = pointers[i];
				pointers[i]->onClick ();
				pointers[i]->isClicked = true;

				missClicked = false;

				if (pointers[i]->advancedMode) break;
			}
			else
			{
				pointers[i]->isClicked = false;

				missClicked = true;
			}
		}
	}

	if (missClicked == true) activeWindow = NULL;
}

Vector Window::getRelativeMousePos (bool coordinatsWithHandle)
{
    return {(double)txMouseX() - getAbsCoordinats(coordinatsWithHandle).x, (double) txMouseY () - getAbsCoordinats(coordinatsWithHandle).y};    
}

Vector Window::getAbsCoordinats (bool coordinatsWithHandle /*=false*/)
{
	Vector coordinats = {};

	Manager *copyOfManager = manager;

	coordinats += rect.pos;

	for (;;)
	{
		if (!copyOfManager) break;

		coordinats = coordinats + copyOfManager->rect.pos;
		if (copyOfManager->coordinatSysFromHandle && !coordinatsWithHandle) coordinats.y += copyOfManager->handle.rect.finishPos.y;
		copyOfManager = copyOfManager->manager;

	}

	return coordinats;
}

Rect Window::getAbsRect (bool coordinatsWithHandle /*=false*/)
{
	Rect coordinats = {};

	Manager *copyOfManager = manager;

	coordinats.pos += rect.pos;

	for (;;)
	{
		if (!copyOfManager) break;

		coordinats.pos += copyOfManager->rect.pos;
		if (copyOfManager->coordinatSysFromHandle && !coordinatsWithHandle)
		{
			coordinats.pos.y += copyOfManager->handle.rect.finishPos.y;
		}
		copyOfManager = copyOfManager->manager;
	}

	coordinats.finishPos = rect.finishPos + (coordinats.pos - rect.pos);

	return coordinats;
}


HDC CLoadManager::loadImage(const char* path)
{
	bool newImage = true;
	int suitableDCNum = -1;
	for (int i = 0; i < currentImagesAmount; i++)
	{
		if (!strcmp(path, images[i].path))
		{
			newImage = false;
			suitableDCNum = i;
			break;
		}
	}

	if (!newImage)
	{
		return images[suitableDCNum].dc;
	}

	
	images[currentImagesAmount].dc = txLoadImage(path);
	images[currentImagesAmount].path = path;
	currentImagesAmount++;

	return images[currentImagesAmount-1].dc;


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


void OpenManager::onClick ()
{
	if (!isClicked)
	{
		manager->advancedMode = !manager->advancedMode;
		manager->draw ();
	}
}

void OpenManager::draw()
{
	$s
	txSetAllColors (color, finalDC);
	//txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	//if (dc) txBitBlt (rect.pos.x, rect.pos.y, dc);
	txBitBlt (finalDC, 0, 0, 0, 0, dc);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		//txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
}

void engine (Manager *manager)
{

	for (;;)
	{
		txSetAllColors (BackgroundColor);
		txRectangle (0, 0, 2000, 2000);
		manager->draw ();
		if (manager->finalDC) txBitBlt (manager->rect.pos.x, manager->rect.pos.x, manager->finalDC);
		if (txMouseButtons () == 1 && manager->rect.inRect (txMouseX (), txMouseY ()))
		{
			manager->onClick ();
			if (!IsRunning) break;
		}

		
		txSleep (0);

	}
}

void txSetAllColors (COLORREF color, HDC dc /*= txDc ()*/, int thickness)
{
	txSetFillColor (color, dc); 
	txSetColor (color, thickness, dc);
}

void RECTangle (const Rect rect, HDC dc /* = txDc ()*/)
{
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}

void LaysMenu::draw()
{
	txSetAllColors(TX_BLACK, finalDC);
	txRectangle(0, 0, DCMAXSIZE, DCMAXSIZE, finalDC);
	char text[30] = {};



	if (canvasManager->activeCanvas != NULL)
	{

		for (int i = 0; i <= canvasManager->activeCanvas->currentLayersLength; i++)
		{
			sprintf(text, "���� %d", i + 1);

			txSetTextAlign(TA_CENTER, finalDC);
			txSetAllColors(TX_WHITE, finalDC);
			txSelectFont("Arial", 18, 5, FW_DONTCARE, false, false, false, 0, finalDC);
			txSelectFont("Arial", 18, 5, FW_DONTCARE, false, false, false, 0, finalDC);
			txDrawText(sectionWidth + rect.getSize().x * 0.05, handle.rect.getSize().y + sectionWidth * i, rect.getSize().x, handle.rect.getSize().y + sectionWidth * (i + 1), text, DT_VCENTER, finalDC);

			txLine(0, handle.rect.getSize().y + sectionWidth * (i), rect.getSize().x, handle.rect.getSize().y + sectionWidth * (i), finalDC);
			
		}
	}
	if (test1)printBlt(finalDC);
	handle.print(finalDC);
	controlHandle();
	txRectangle(0, 0, SIDETHICKNESS, rect.getSize().y, finalDC);
	txRectangle(0, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, rect.getSize().y, finalDC);
	txRectangle(rect.getSize().x - SIDETHICKNESS, rect.getSize().y - SIDETHICKNESS, rect.getSize().x, 0, finalDC);
	if (test1)printBlt(finalDC);
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
				if (canvasManager->activeCanvas->historyOfDrawingMode[canvasManager->activeCanvas->currentHistoryNumber - 1] > 0 && canvasManager->activeCanvas->currentHistoryNumber - 1 - i >= 0)	
				{
					txBitBlt (finalDC, (toolHDCSize / 4), handle.rect.getSize().y + (toolHDCSize) * i + (toolHDCSize / 4), 0, 0, toolsDC[canvasManager->activeCanvas->historyOfDrawingMode[canvasManager->activeCanvas->currentHistoryNumber - 1 - i] - 1]);
					tool = canvasManager->activeCanvas->historyOfDrawingMode[canvasManager->activeCanvas->currentHistoryNumber - 1 - i];
				}
		
				if (canvasManager->activeCanvas->currentHistoryNumber - 1 - i < 0 && canvasManager->activeCanvas->historyOfDrawingMode[10 +canvasManager->activeCanvas->currentHistoryNumber - 1 - i]  >  0)
				{
					txBitBlt (finalDC, (toolHDCSize / 4), handle.rect.getSize().y + (toolHDCSize) * i + (toolHDCSize / 4), 0, 0, toolsDC[canvasManager->activeCanvas->historyOfDrawingMode[10 +canvasManager->activeCanvas->currentHistoryNumber - 1 - i] - 1]);	
					tool = canvasManager->activeCanvas->historyOfDrawingMode[10 +canvasManager->activeCanvas->currentHistoryNumber - 1 - i];
				}
				//txSetTextAlign (TA_LEFT);
				//char *num = new char[canvasManager->activeCanvas->currentHistoryLength]{};
				char strNum[10] = {};
				sprintf (strNum, "%d", canvasManager->activeCanvas->currentHistoryLength - i);
				

				char toolName[50] = {};
				if (tool == 1)
				{
					sprintf (toolName, "�����");
				}
				if (tool == 2)
				{
					sprintf (toolName, "������");
				}
				if (tool == 4)
				{
					sprintf (toolName, "�����");
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


void History::onClick ()
{
	int mx = txMouseX();
	int my = txMouseY ();
	if (advancedMode && !isClicked)
	{
		clickHandle ();

		if (canvasManager->activeCanvas != NULL)
		{
			for (int i = 0; i < canvasManager->activeCanvas->currentHistoryLength; i++)
			{
			

					Rect button = {.pos = {getAbsCoordinats().x + i, getAbsCoordinats().y + handle.rect.getSize().y + i *toolHDCSize}, .finishPos = {getAbsCoordinats().x + rect.getSize().x, getAbsCoordinats().y + handle.rect.getSize().y +  (i + 1) * toolHDCSize}};
					if (button.inRect (mx, my))
					{
						int saveCurrentHistoryNumber = canvasManager->activeCanvas->currentHistoryNumber;
						canvasManager->activeCanvas->currentHistoryNumber -= (i);
						if (canvasManager->activeCanvas->currentHistoryNumber < 0) canvasManager->activeCanvas->currentHistoryNumber += 9;
						canvasManager->activeCanvas->currentHistoryLength -= (i);

						//if (i != 0)canvasManager->activeCanvas->canvas = canvasManager->activeCanvas->history[canvasManager->activeCanvas->currentHistoryNumber];
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

void Window::print (HDC DC)
{
	draw();
	txBitBlt (DC, rect.pos.x, rect.pos.y, rect.getSize().x, rect.getSize().y, finalDC);
}

Vector Window::getSize()
{
	return this->rect.finishPos - this->rect.pos;
}

void Window::draw ()
{
	$s
	if (finalDC) txSetAllColors (color, finalDC);
	if (finalDC) txRectangle (0, 0, rect.getSize ().x, rect.getSize ().y, finalDC);
	if (test1)printBlt (finalDC);


	if (finalDC) txSetAllColors (TextColor, finalDC);
	txSetTextAlign (TA_CENTER, finalDC);
	//txDrawText (0, 0, rect.getSize().x, rect.getSize().y, text, DT_CENTER, finalDC);
	txTextOut (rect.getSize().x / 2, rect.getSize().y * 0.05 , text, finalDC);
	if (test1)printBlt (finalDC);


	if (dc) 
	{
		//txBitBlt (finalDC, rect.size.x, rect.size.y, originalRect.size.x, originalRect.size.y, dc);	
		compressDraw (finalDC, {0, 0}, rect.getSize (), dc, originalRect.getSize (), __LINE__);
	}
}

void Window::deleteButton ()
{

	//printBlt (dc);
	////if (dc) txDeleteDC (dc);
	///=if (finalDC) txDeleteDC (finalDC);
}

void Window::resize (Rect newRect)
{
	
	if (!dc) return;
	//compressImage (dc, {newRect.getSize().x, newRect.getSize().y}, {rect.getSize().x, rect.getSize().y});
	if (test1) printBlt (dc);
	rect = newRect;
}

void Window::resize (Vector newSize, Vector oldSize)
{
	
	if (!dc) return;
	//compressImage (dc, newSize, {rect.getSize().x, rect.getSize().y});
	//if (test1) printBlt (dc);
	rect = {.pos = rect.pos * newSize / oldSize, .finishPos = rect.finishPos * newSize / oldSize};
}

void Window::setStartRect (Vector pos, Vector finishPos)
{
	rect = {pos, finishPos};
	originalRect = rect;
}


void CanvasManager::draw ()
{
	txSetAllColors (BackgroundColor, finalDC);
	txRectangle (0, 0, 3000, 3000, finalDC);


	//bar


	//if (dc) txBitBlt (finalDC, 0, 0, 0, 0, dc);
	if (activeCanvas)bar->setProgress(&activeCanvas->filter->totalProgress, &activeCanvas->filter->currentProgress);
	if (newCanvas.isClicked)
	{
		addWindow(new Canvas({ .pos = {100, 50}, .finishPos = {600, 500 + 50} }, closeCanvasButton));
		newCanvas.isClicked = false;
	}
	
	newCanvas.draw();
	txBitBlt(finalDC, newCanvas.rect.pos.x, newCanvas.rect.pos.y, newCanvas.rect.getSize().x, newCanvas.rect.getSize().y, newCanvas.finalDC);


	for (int i = 0; i < newButtonNum; i++)
	{
		pointers[i]->draw ();

 		if (pointers[i]->advancedMode) txBitBlt (finalDC, pointers[i]->rect.pos.x, pointers[i]->rect.pos.y, pointers[i]->rect.getSize().x, pointers[i]->rect.getSize().y, pointers[i]->finalDC); 

		if (txMouseButtons () != 1)
		{
			pointers[i]->isClicked = false;
		}
	}

	if (manager->getActiveWindow () != this && manager) 
	{
		//activeWindow = NULL;
	}
}

void CanvasManager::onClick()
{
	bool missClicked = true;

	int mx = txMouseX();
	int my = txMouseY();

	if (advancedMode)
	{
		if (handle.getAbsRect().inRect(mx, my))
		{
			startCursorPos.x = mx;
			startCursorPos.y = my;
			handle.isClicked = true;
		}

		if (newCanvas.getAbsRect().inRect (mx, my) && !newCanvas.isClicked && !isClicked)
		{
			newCanvas.isClicked = true;
			missClicked = false;
		}
		else
		{
			newCanvas.isClicked = false;

			missClicked = true;
		}

		for (int i = newButtonNum - 1; i >= 0; i--)
		{
			if (pointers[i]->getAbsRect().inRect(mx, my))
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
}

void CanvasManager::deleteButton()
{
	newCanvas.deleteButton();
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


void Canvas::draw ()
{
	assert (canvas);
	txSetAllColors (BackgroundColor, finalDC);
	if (!nonConfirmFilter)
	{
		txRectangle (0, 0, 3000, 3000, finalDC);
	}
	if (nonConfirmFilter && reCountEnded)
	{
		txRectangle (0, 0, 3000, 3000, finalDC);
	}

	if (!nonConfirmFilter)
	{
		txBitBlt(finalDC, -canvasCoordinats.x, -canvasCoordinats.y, 0, 0, canvas);
	}
	if (nonConfirmFilter && reCountEnded)
	{
		txBitBlt(finalDC, -canvasCoordinats.x, -canvasCoordinats.y, 0, 0, tempFilterDC);
	}

    currentDate->absMouseCoordinats = {txMouseX(), txMouseY ()};
    currentDate->managerPos = getAbsCoordinats();
    currentDate->color = DrawColor;
    currentDate->canvasCoordinats = canvasCoordinats;
    currentDate->gummiThickness = GummiThickness;




	


	controlFilter();

    if (activeTool)
    {
        if (history[currentHistoryNumber - 1].tools->use (currentDate, canvas, history[currentHistoryNumber - 1].toolsData, finalDC))
        {
            activeTool = false;
        }
    }


	if (txGetAsyncKeyState('Q'))
	{
		endtillkey('Q');
		//playHistory ();
		createLay();
	}
	if (txGetAsyncKeyState ('Z')) 
	{
		endtillkey ('Z');
		//playHistory ();
		returnHistory(1);
	}
	//drawLay();

    if (txGetAsyncKeyState ('2'))
    {
        DrawingMode = 2;
    }

	
	if (manager->activeWindow != this) wasClicked = false;

     /*
	if (txMouseButtons () == 2 && wasClicked)
	{
        
		//txSetColor (DrawColor, lineThickness, canvas);
		//txLine (lastClick.x + canvasCoordinats.x, lastClick.y + canvasCoordinats.y, txMouseX () - getAbsCoordinats().x + canvasCoordinats.x, txMouseY () - getAbsCoordinats().y + canvasCoordinats.y, canvas);
		wasClicked = false;

        if (currentHistoryLength < HistoryLength - 1) currentHistoryLength++;

        timesShowedHistoryInRow = 0;

	}

	if (wasClicked && manager->activeWindow != this) wasClicked = false;

	if (wasClicked)
	{
		txSetAllColors (DrawColor, finalDC);
		txSetColor (DrawColor, lineThickness, finalDC);
		txLine (lastClick.x, lastClick.y, txMouseX () - getAbsCoordinats().x, txMouseY () - getAbsCoordinats().y, finalDC);
	}  */

	if (clearBackground)
	{
		//txSetAllColors (BackgroundColor, canvas);
		//txRectangle (0, 0, canvasSize.x, canvasSize.y, canvas);
		//clearBackground = false;
	}

	 
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

void Canvas::returnHistory (int stepsBack)
{
	if (!(stepsBack <= currentHistoryLength)) return; 
	//HDC historyDC = playHistoryDC();
    //printfDCS ();
	copyAndDelete(canvas, playHistoryDC (stepsBack));
    //txDeleteDC (hdc);
    //printfDCS ();
	currentHistoryNumber -= stepsBack;
	if (currentHistoryNumber < 0) currentHistoryNumber += HistoryLength;
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
	if (txGetAsyncKeyState(VK_LEFT)) SecondFilterValue++;
	if (txGetAsyncKeyState(VK_RIGHT)) SecondFilterValue--;
	if (txGetAsyncKeyState(VK_DOWN)) FirstFilterValue+=10;
	if (txGetAsyncKeyState(VK_UP)) FirstFilterValue-=10;

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
	if (txMouseButtons() != 1) isResizing = false;
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
 

void SizeButton::onClick ()
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

void CloseButton::onClick ()
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
	//txBitBlt (history[currentHistoryNumber], 0, 0, 0, 0, canvas);
	timeSavedHistory++;
	if (currentHistoryNumber < HistoryLength - 1) currentHistoryNumber++;
	else currentHistoryNumber = 0;
    //printf ("%d %d\n ", currentHistoryNumber, (currentHistoryNumber < HistoryLength - 1));

    currentHistoryLength++;

	
	if (timeSavedHistory > HistoryLength - 1)
	{
		int newLastStep = currentHistoryNumber;
		if (newLastStep >= HistoryLength)
		{
			newLastStep = 0;
		}
		lastSavedDC = playHistoryDC(9);
		printBlt(lastSavedDC);
	}
	
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

bool Canvas::controlLay ()
{
	if (DrawingMode == 4 && currentLayersLength > 0)
	{
		//if (!isClicked) saveHistory ();
 		//txSetAllColors (DrawColor, layers[activeLayNum]);

		//RGBQUAD *c = & layersBuf[activeLayNum][(int) (lastClick.x - canvasCoordinats.x - layersCoordinats[activeLayNum].x) + (int)((DCMAXSIZE - (lastClick.y - canvasCoordinats.y - layersCoordinats[activeLayNum].y)) * DCMAXSIZE)];
		//c->rgbReserved = 255;
		
 		//txEllipse (lastClick.x + canvasCoordinats.x - 5, lastClick.y + canvasCoordinats.y - 5, lastClick.x + 5 + canvasCoordinats.x, lastClick.y + canvasCoordinats.y + 5, layers[currentLayersNum - 1]);
		//txSetPixel (lastClick.x - canvasCoordinats.x - layersCoordinats[activeLayNum].x, lastClick.y - canvasCoordinats.y - layersCoordinats[activeLayNum].y, DrawColor, layers[activeLayNum]);
		//if (lastClick.x - canvasCoordinats.x + lay[activeLayNum].layCoordinats.x >= 0 && lastClick.y - canvasCoordinats.y + lay[activeLayNum].layCoordinats.y >= 0)
			//lay[activeLayNum].circle (lastClick.x - canvasCoordinats.x - lay[activeLayNum].layCoordinats.x, lastClick.y - canvasCoordinats.y - lay[activeLayNum].layCoordinats.y, 10);
		lay[activeLayNum].circle (lastClick.x - canvasCoordinats.x, lastClick.y - canvasCoordinats.y, 10);

		//printBlt ();
		return true;
	}

	if (DrawingMode == 5 && currentLayersLength > 0)
	{
		cursorPos.x = txMouseX ();
		cursorPos.y = txMouseY ();
		lay[activeLayNum].isClicked = true;

		return true;
	}
	return false;
}

void Canvas::drawLay ()
{
	//txSetWindowsHook
	if (txGetAsyncKeyState(VK_RIGHT) && currentLayersLength > 0) lay[activeLayNum].layCoordinats += {1, 0};		
	if (txGetAsyncKeyState(VK_LEFT) && currentLayersLength > 0) lay[activeLayNum].layCoordinats += {-1, 0};		
	if (txGetAsyncKeyState(VK_DOWN) && currentLayersLength > 0) lay[activeLayNum].layCoordinats += {0, 1};		
	if (txGetAsyncKeyState(VK_UP) && currentLayersLength > 0) lay[activeLayNum].layCoordinats += {0, -1};
	if (txGetAsyncKeyState ('U') && activeLayNum < currentLayersLength - 1) activeLayNum++;
	if (txGetAsyncKeyState ('J') && activeLayNum > 0) activeLayNum--;

	if (lay[activeLayNum].isClicked)
	{
		printf ("%lf\n", lay[activeLayNum].layCoordinats.x);
		lay[activeLayNum].layCoordinats.x += txMouseX () - cursorPos.x;
		lay[activeLayNum].layCoordinats.y += txMouseY () - cursorPos.y;
		cursorPos.x = txMouseX (); 
		cursorPos.y = txMouseY (); 
	}
	if (txMouseButtons () != 1)
	{
		lay[activeLayNum].isClicked = false;
	}

	for (int i = 0; i < currentLayersLength; i++)
	{
		txAlphaBlend (finalDC, lay[i].layCoordinats.x - canvasCoordinats.x, lay[i].layCoordinats.y - canvasCoordinats.y, 0, 0, lay[i].lay);
		//if (test1)printBlt (layers[i]);
		//if (test1)printBlt (copyDC);
	}	
	//if (currentLayersNum > 0) txAlphaBlend (finalDC, laysersCoordinats[currentLayersNum-1].x, laysersCoordinats[currentLayersNum-1].y, 0, 0, layers[currentLayersNum - 1]);
	//if (currentLayersNum > 0) txAlphaBlend (finalDC, 0, 0, 0, 0, copyDC);
	//if (test1)printBlt (copyDC);
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

void Lay::createLay	()
{
	lay = txCreateDIBSection (DCMAXSIZE, DCMAXSIZE, &layBuf);

	for (int y = 0; y < DCMAXSIZE; y++)
	{
		for (int x = 0; x < DCMAXSIZE; x++)
		{
			RGBQUAD* copy = &layBuf[x + y * DCMAXSIZE];
			//copy->rgbRed      = (BYTE) 0;
			//copy->rgbGreen    = (BYTE) 0;
			//copy->rgbBlue     = (BYTE) 0;
			copy->rgbReserved = (BYTE) 0;
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


void Lay::line(int x0, int y0, int x1, int y1, COLORREF drawColor) 
{
	
	bool steep = false;
	if (abs (x0 - x1) < abs (y0 - y1)) 
	{
		//swap (x0, y0);
		//swap (x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		//swap (x0, x1);
		//swap (y0, y1);
	}
	int dx = x1-x0;
	int dy = y1-y0;
	int derror2 = abs (dy) * 2;
	int error2 = 0;
	int y = y0;

	for (int x = x0; x <= x1; x++)
	{
		if (steep) 
		{
			RGBQUAD* color = &layBuf[(int) (y + (DCMAXSIZE - x) * DCMAXSIZE)];
			color->rgbRed = txExtractColor (drawColor, TX_RED);
			color->rgbGreen = txExtractColor (drawColor, TX_GREEN);
			color->rgbBlue = txExtractColor (drawColor, TX_BLUE);
			color->rgbReserved = 255;
		} 
		else 
		{
			RGBQUAD* color = &layBuf[(int) (x + (DCMAXSIZE - y) * DCMAXSIZE)];
			color->rgbRed = txExtractColor (drawColor, TX_RED);
			color->rgbGreen = txExtractColor (drawColor, TX_GREEN);
			color->rgbBlue = txExtractColor (drawColor, TX_BLUE);
			color->rgbReserved = 255;
		}
		error2 += derror2;

		if (error2 > dx) 
		{
			y += (y1 > y0 ? 1 : -1);

			error2 -= dx * 2;
		}
	}
}

bool Vignette::use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC)
{
    Vector pos = data->absMouseCoordinats - data->managerPos + data->canvasCoordinats;
    firstUse (data, output, pos);
    DrawColor = txGetPixel (pos.x, pos.y, canvas);

    ColorSave colorsave (DrawColor);
    *(ColorSave*)output = colorsave;

    finishUse ();

    return true;
}

void Vignette::load (void* input, HDC finalDC)
{
    ColorSave* toolDate = (ColorSave*)input;
    DrawColor = toolDate->color;
}

bool Gummi::use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC)
{
    Vector pos = data->absMouseCoordinats - data->managerPos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
    }
    txSetAllColors(data->backGroundColor, tempDC, data->size.x);
	txEllipse (pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, tempDC);

    

    //printf ("1");
    
    if (txMouseButtons () != 1 || pos != startPos) 
    {
        txSetAllColors(data->backGroundColor, canvas, data->size.x);
	    txEllipse (pos.x - data->gummiThickness + data->canvasCoordinats.x, pos.y - data->gummiThickness + data->canvasCoordinats.y, pos.x + data->gummiThickness + data->canvasCoordinats.x, pos.y + data->gummiThickness + data->canvasCoordinats.y, canvas);

        workedLastTime = false;

        ToolSave saveTool (startPos + data->canvasCoordinats, {(double) data->gummiThickness, (double) data->gummiThickness}, data->backGroundColor, data->size.x, (const char*)2);

	    *(ToolSave*)output = saveTool;

        return true;
    }

    return false;
}

bool RectangleTool::use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC)
{
    Vector pos = data->absMouseCoordinats - data->managerPos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
    }

    txSetAllColors(data->color, tempDC, data->size.x);       
    txRectangle (startPos.x, startPos.y, pos.x, pos.y, tempDC);

    if (txMouseButtons () == 2)
    {
        txSetAllColors(data->color, canvas, data->size.x);       
        txRectangle (startPos.x, startPos.y, pos.x, pos.y, canvas);

        ToolSave saveTool (startPos + data->canvasCoordinats, pos - startPos, data->color, data->size.x, (const char*)2);
        (*(ToolSave*)output) = saveTool;
        workedLastTime = false;
        return true;
    }

    return false;
}

void RectangleTool::load (void* input, HDC finalDC)
{
    ToolSave* rectDate = (ToolSave*)input;
    txSetAllColors (rectDate->color, finalDC, rectDate->thickness);


	txRectangle (rectDate->pos.x, rectDate->pos.y, rectDate->pos.x + rectDate->size.x, rectDate->pos.y + rectDate->size.y, finalDC);   
}

bool Point::use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC)
{
    Vector pos = data->absMouseCoordinats - data->managerPos;
    if (firstUse (data, output, pos))
    {
        if (pointSave) delete(pointSave);
        pointSave = new PointSave(100);
    }
    txSetAllColors(data->color, tempDC, data->size.x);
	txEllipse (pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, tempDC);

    if (lastPos != pos)
    {
        txSetAllColors(data->color, canvas, data->size.x);
	    txEllipse (pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, canvas);

        ToolSave saveTool (pos + data->canvasCoordinats, data->size, data->color, data->size.x, (const char*)2);
        pointSave->addPoint (saveTool);
    }
    
    if (txMouseButtons () != 1) 
    { 
        *((PointSave*)output) = *pointSave;

        finishUse ();

        return true;
    }

    lastPos = pos;

    return false;
}

void Point::load (void* input, HDC finalDC)
{
    PointSave* pointsDate = (PointSave*)input;
    if (pointsDate->currentLength > 0) txSetAllColors (pointsDate->points[1].color, finalDC, pointsDate->points[1].thickness);

    for (int i = 0; i < pointsDate->currentLength; i++)
    {
	    txEllipse (pointsDate->points[i].pos.x - pointsDate->points[i].size.x, pointsDate->points[i].pos.y - pointsDate->points[i].size.y,pointsDate->points[i].pos.x + pointsDate->points[i].size.x, pointsDate->points[i].pos.y + pointsDate->points[i].size.y, finalDC);
    }

}

bool Tool::firstUse (ProgrammeDate *data, void* output, Vector currentPos)
{
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = currentPos;
        return true;
    }
    return false;
}

void Tool::finishUse ()
{
    workedLastTime = false;
}


bool Tool::use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC)
{
    Vector pos = data->absMouseCoordinats - data->managerPos;
    firstUse(data, output, pos);
    txSetAllColors(data->color, tempDC, data->size.x);
	txEllipse (pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, tempDC);

    

    //printf ("1");
    
    if (txMouseButtons () != 1 || pos != startPos) 
    {
        txSetAllColors(data->color, canvas, data->size.x);
	    txEllipse (pos.x - data->size.x + data->canvasCoordinats.x, pos.y - data->size.y + data->canvasCoordinats.y, pos.x + data->size.x + data->canvasCoordinats.x, pos.y + data->size.y + data->canvasCoordinats.y, canvas);

        workedLastTime = false;

        ToolSave saveTool (pos + data->canvasCoordinats, data->size, data->color, data->size.x, (const char*)2);

	    *(ToolSave*)output = saveTool;
        return true;
    }

    return false;
}

void Tool::load (void* input, HDC finalDC)
{
    ToolSave* toolDate = (ToolSave*)input;
    txSetAllColors (toolDate->color, finalDC, toolDate->thickness);
	txEllipse (toolDate->pos.x - toolDate->size.x, toolDate->pos.y - toolDate->size.y, toolDate->pos.x + toolDate->size.x, toolDate->pos.y + toolDate->size.y, finalDC);
}


int ToolData::getByteSize ()
{

    byteLength = sizeof (*this);
    return byteLength;
}

void PointSave::addPoint (ToolSave &point)
{
    points[currentLength].pos = point.pos;
    points[currentLength].size = point.size;
    points[currentLength].color = point.color;
    points[currentLength].thickness = point.thickness;
    currentLength++;
}

int PointSave::getByteSize ()
{
    byteLength = sizeof (*this) + Length * sizeof (ToolSave);
    return byteLength;
}

bool Line::use (ProgrammeDate *data, HDC canvas, void* output, HDC tempDC)
{
    Vector pos = data->absMouseCoordinats - data->managerPos;
    if (!workedLastTime) 
    {
        startPos = pos;
        workedLastTime = true;
    }
	txSetAllColors(data->color, tempDC, data->size.x);
	txLine (startPos.x, startPos.y, pos.x, pos.y, tempDC);

    if (txMouseButtons () == 2)
    {
        txSetAllColors(data->color, canvas, data->size.x);
	    txLine (startPos.x + data->canvasCoordinats.x, startPos.y + data->canvasCoordinats.y, pos.x + data->canvasCoordinats.x, pos.y + data->canvasCoordinats.y, canvas);
        ToolSave saveTool (startPos + data->canvasCoordinats, pos - startPos, data->color, data->size.x, (const char*)1);

	    *(ToolSave*)output = saveTool;

        finishUse ();
        return true;

    }
    return false;
}

void Line::load (void* input, HDC finalDC)
{
	ToolSave* toolDate = (ToolSave*)input;
    txSetAllColors (toolDate->color, finalDC, toolDate->thickness);
	txLine (toolDate->pos.x, toolDate->pos.y, toolDate->pos.x + toolDate->size.x, toolDate->pos.y + toolDate->size.y, finalDC);

}


void Lay::circle (int x0, int y0, int r)
{
	for (double t = 0; t <= 1; t += 0.001)
	{
		double x = x0 - r + t * 2 *(r);
		int y1 = sqrt (r * r - (x - x0) * (x - x0)) + y0;
		int y2 = -sqrt (r * r - (x - x0) * (x - x0)) + y0;

		//printf ("x: %lf y = {%lf, %lf}\n", x, y1, y2);
		txSetAllColors (TX_RED, lay);
		//txSetColor (TX_RED, 2);
		//txLine (x, y1, x, y2, lay);
		if (x >= 0 && y1 >= 0 && y2 >= 0)
		line (x, y1, x, y2);
		//if (test1)printBlt (lay);





		//txSetPixel (x, y, DrawColor, lay);
		//txSetAllColors (DrawColor, lay);
		//txEllipse (x- 3, y-3, x + 3, y + 3, lay);
	}

}





void Canvas::createLay ()
{

	lay[currentLayersLength].createLay ();
	//layers[currentLayersLength] = txCreateDIBSection (DCMAXSIZE, DCMAXSIZE, &layersBuf[currentLayersLength]);
	//printBlt (layers[currentLayersNum]);

	/*
	for (int y = 0; y < DCMAXSIZE; y++)
	{
		for (int x = 0; x < DCMAXSIZE; x++)
		{
			RGBQUAD* copy = &layersBuf[currentLayersLength][x + y * DCMAXSIZE];
			copy->rgbRed      = (BYTE) 0;
			copy->rgbGreen    = (BYTE) 0;
			copy->rgbBlue     = (BYTE) 0;
			copy->rgbReserved = (BYTE) 0;
		}
	}
	*/
	//txSetAllColors (TX_RED, layers[currentLayersNum]);
	//txRectangle (0, 0, 1000, 1000, layers[currentLayersNum]);
	//txClear ();
	//txAlphaBlend (txDC(), 0, 0, 300, 300, layers[currentLayersNum]);
	//txSleep (3000);
	//stop;
	//endtillkey ('Q');
	//txSleep (3000);
	//printBlt (layers[currentLayersNum]);

	currentLayersLength++;
	activeLayNum = currentLayersLength - 1;
	
}

HDC Canvas::playHistoryDC (int stepBack)
{
	/*
	if (timesShowedHistoryInRow == HistoryLength) return;
	if (currentHistoryLength <= 1) return; 
	if (currentHistoryNumber > 0)
	{
		//canvas = history[--currentHistoryNumber];
		currentHistoryLength--;
	}
	else 
	{
		currentHistoryNumber = HistoryLength;
		//canvas = history[--currentHistoryNumber];
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
			//if (currentHistoryLength == HistoryLength - 1)  pos = currentHistoryNumber + 1 + i;
			if (currentHistoryLength < HistoryLength - 1) pos = currentHistoryNumber - 2 - i;
			if (pos >= HistoryLength) pos -= HistoryLength;
            history[pos].tools->load (history[pos].toolsData, historyDC);
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

void Canvas::onClick ()
{
	txSetAllColors ( drawColor);
	lastClick = { .x = txMouseX() - getAbsCoordinats().x, .y = txMouseY() - getAbsCoordinats().y };
	
	int mx = txMouseX();
	int my = txMouseY();

	if (manager->activeWindow == this)
	{
		if (scrollBarVert.getAbsRect().inRect(mx, my))
		{
			scrollBarVert.onClick();
			return;
		}
		if (scrollBarHor.getAbsRect().inRect(mx, my))
		{
			scrollBarHor.onClick();
			return;
		}

		if (closeCanvas.getAbsRect().inRect(mx, my))
		{
			advancedMode = false;
			return;
		}
	}

	
	
	if (!(isClicked) && manager->activeWindow == this)
	{
		if (handle.getAbsRect().inRect(mx, my))
		{
			Rect rightTop = { rect.finishPos.x - 0.2 * rect.getSize().x, rect.pos.y, rect.finishPos.x, rect.pos.y + 0.2 * rect.getSize().y};
			if (rightTop.inRect(mx, my))
			{
				startResizingCursor = { (double)mx, (double)my };
				isResizing = true;
				return;
			}
			startCursorPos = { (double)mx, (double)my };
			handle.isClicked = true;
			return;
		}

		//if (DrawingMode == 1 && !nonConfirmFilter) wasClicked = true;
	}

	//if (controlLay ()) return;
	
    /*
		if (DrawingMode == 2 && !nonConfirmFilter)
		{
			// saveHistory({20, 20});
			txSetAllColors(BackgroundColor, canvas);
			txEllipse(lastClick.x + canvasCoordinats.x - 20, lastClick.y + canvasCoordinats.y - 20, lastClick.x + canvasCoordinats.x + 20, lastClick.y + canvasCoordinats.y + 20, canvas);
		}

		if (DrawingMode == 3 && !nonConfirmFilter)
		{
			//RGBQUAD pixel = canvasArr[(int)(lastClick.x + canvasCoordinats.x) + (int) ((canvasSize.y - (lastClick.y + canvasCoordinats.y)) * canvasSize.x)];
			DrawColor = txGetPixel (lastClick.x + canvasCoordinats.x, lastClick.y + canvasCoordinats.y, canvas);
			//DrawColor = RGB (pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue);
			//printf ("%d\n", DrawColor);
		}
		if (DrawingMode == 4 && !nonConfirmFilter)
		{
			//saveHistory({20, 20});
			//txSetAllColors (DrawColor, canvas);

			//txEllipse (lastClick.x + canvasCoordinats.x - 5, lastClick.y + canvasCoordinats.y - 5, lastClick.x + 5 + canvasCoordinats.x, lastClick.y + canvasCoordinats.y + 5, canvas);
		}
    */
    if (!activeTool)
    {
        saveHistory();
	    txSetAllColors (DrawColor, canvas);
       // int addNewHistoryNum = currentHistoryNumber - 1;
	    //if (addNewHistoryNum < 0) addNewHistoryNum += 10;
        //history[addNewHistoryNum].tools = tools[1];
        //assert (history[addNewHistoryNum].tools);
        history[currentHistoryNumber - 1].tools = ToolManager.tools[DrawingMode - 1];
        history[currentHistoryNumber - 1].toolsData = new char [ToolManager.tools[DrawingMode - 1]->ToolSaveLen];
        currentDate->size = {lineThickness, lineThickness};
        activeTool = true;
        if (history[currentHistoryNumber - 1].tools->use (currentDate, canvas, history[currentHistoryNumber - 1].toolsData, finalDC))
        {
            activeTool = false;
        }
    }
    
}

void ColorButton::onClick ()
{
	DrawColor = color;
}



void CleanButton::onClick ()
{
	mainCanvas->clearBackground = true;
}