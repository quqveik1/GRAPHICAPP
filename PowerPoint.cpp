
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"
#include <cmath>



const int DELTACLOCKTIME = 100;
double Brightness = 0;   
double SizeKForCloseCanvas = 0.05;
COLORREF BackgroundColor = TX_BLACK;
COLORREF DrawColor = TX_RED;
int DrawingMode = 1;
const int DCMAXSIZE = 1000;

double IncomeBrightness = 255;


HDC TestPhoto;


#define printBlt(image)         \
{                               \
    assert (image);				\
	txSetAllColors (TX_RED);		\
	txRectangle (0, 0, 1000, 1000);	 \
	txBitBlt (0, 0, image); 		   \
	txSleep (0);								\
    _getch ();                  \
    txClear ();                 \
}

#define copyOnDC(x, y, image)	\
{										  \
	txBitBlt (finalDC, x, y, 0,0, image); \
}										  \

#define drawOnFinalDC(button)					  \
{												  \
	button.draw ();								  \
	txBitBlt (finalDC, button.rect.pos.x, button.rect.pos.y, button.rect.getSize().x, button.rect.getSize().y, button.finalDC);  \
}												  \



void compressImage (HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize);
void txSetAllColors (COLORREF color, HDC dc = txDC ());


struct Manager;

struct Window
{
	Rect rect;
	COLORREF color;
	const char *text;
	bool isClicked;
	HDC dc;
	HDC finalDC;
	RGBQUAD *finalDCArr;
	Manager *manager;
	bool advancedMode;

	Window (Rect _rect, COLORREF _color = TX_WHITE, HDC _dc = NULL, Manager *_manager = NULL, const char *_text = "", bool _advancedMode = true) :
		rect (_rect),
		color(_color),
		manager (_manager),
		text (_text), 
		isClicked (false), 
		dc (_dc),
		advancedMode (_advancedMode)
	{
		if (_rect.getSize().x > 0 && _rect.getSize().y > 0)
		{
			finalDC = txCreateDIBSection(DCMAXSIZE, DCMAXSIZE, &finalDCArr);
			txSetAllColors(color, finalDC);
			txRectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);
		}
	}


	Vector getAbsCoordinats ();
	Rect getAbsRect ();
	void deleteButton ();
	void resizeDC (Vector newSize);

	virtual void draw ();
	virtual void onClick () {};
};

struct Manager : Window
{
	int length;
	Window **pointers = NULL;
	int newButtonNum;
	Window *activeWindow;
	Window handle;
	Vector startCursorPos;

	Manager (Rect _rect,  int _length, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}, COLORREF _color = TX_WHITE) : 
		Window (_rect, _color, _dc, NULL, "", _advancedMode),
		length (_length),
		pointers (new Window*[length]{}),
		newButtonNum (0),
		activeWindow (NULL), 
		handle (_handle),
		startCursorPos({})
	{
		handle.manager = this;
		handle.rect.finishPos.x = DCMAXSIZE * 10;
	}

	bool addWindow (Window *window);
	Window *getActiveWindow ();
	void controlHandle ();
	void replaceWindow (int numOfWindow);
	void controlSize ();

	virtual void draw () override;
	virtual void onClick () override;
};

struct ToolsPalette : Manager
{
	int lastSelected = 0;
	ToolsPalette (Rect _rect, int _length) :
		Manager (_rect, _length)
	{
	}

	virtual void onClick() override;
	virtual void draw() override;
};

struct Canvas : Manager
{
	HDC canvas;
	double lineThickness = 3;
	COLORREF drawColor = TX_RED;
	Window closeCanvas;
	bool clearBackground = true;
	Vector canvasCoordinats;
	Vector canvasSize;
	RGBQUAD *canvasArr;
	bool confirmBrightness;
	Vector startResizingCursor = {};
	bool isResizing = false;

	bool wasClicked = false;
	Vector lastClick = {};

	Canvas (Rect _rect, HDC _closeDC = NULL) : 
		Manager (_rect, 0, true, NULL, {.pos = {0, 0}, .finishPos = {_rect.getSize ().x, _rect.getSize ().y * 0.05}}),
		canvasCoordinats ({}),
		canvasSize({DCMAXSIZE, DCMAXSIZE}),
		confirmBrightness (false),
		closeCanvas ({ .pos = {0, 0}, .finishPos = {_rect.getSize().x * SizeKForCloseCanvas, _rect.getSize().y * SizeKForCloseCanvas} }, TX_RED, _closeDC, this, "X")
	{
		canvas = txCreateDIBSection (canvasSize.x, canvasSize.y, &canvasArr);
	}

	void controlSize();

	virtual void draw () override;
	virtual void onClick () override;
};

struct TimeButton : Window
{
	TimeButton (Rect _rect)	:
		Window (_rect)
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
	{}

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

	CanvasManager (Rect _rect, HDC _NewCanvasDC) : 
		Manager (_rect, 10, true, NULL, {}, TX_BLACK),
		newCanvas ({.pos = {450, 0}, .finishPos = {500, 50}}, TX_WHITE, _NewCanvasDC), 
		sizeOfNewCanvas ({500, 500})
	{
		compressImage (closeCanvasButton, {sizeOfNewCanvas.x * SizeKForCloseCanvas, sizeOfNewCanvas.y * SizeKForCloseCanvas}, txLoadImage("CloseButton2.bmp"), {512, 512});
	}

	virtual void draw () override;
	virtual void onClick() override;
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

struct Slider : Manager 
{
	Window arrow1;
	Window arrow2;
	Window sliderQuadrate;
	//HDC QuadrateDC;
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
	Window upSideOfQuadrateSlider;
	Window bottomSideOfQuadrateSlider;

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
		isWhite (_isWhite),
		upSideOfQuadrateSlider ({}), 
		bottomSideOfQuadrateSlider ({})
	{
		//compressImage (dc, {rect.getSize ().x, rect.getSize ().y}, _dc, {_originalSize.x, _originalSize.y}

		arrow1.manager = this;
		arrow2.manager = this;
		sliderQuadrate.manager = this;
		upSideOfQuadrateSlider.manager = this;
	    bottomSideOfQuadrateSlider.manager = this;

		if (horizontalMode)
		{
			arrow1.rect.pos.x = 0;
			arrow1.rect.pos.y = 0;
			arrow1.rect.finishPos.x = (1.3) * (rect.getSize ().y);
			arrow1.rect.finishPos.y = rect.getSize ().y;
			compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1Hor.bmp"), {13, 17});
			//fif (!isWhite) compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1Hor.bmp"), {13, 17});
			if (isWhite) 
			{
				arrow1.rect.finishPos.x = rect.pos.x + (1) * arrow1.rect.getSize().y;
				compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1WhiteVert.bmp"), {16, 16});
			}


			arrow2.rect.pos.x = rect.getSize ().x - (1.3) * rect.getSize ().y;
			arrow2.rect.pos.y = 0;
			arrow2.rect.finishPos.x = rect.getSize ().x;
			arrow2.rect.finishPos.y = rect.getSize ().y;
			compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2Hor.bmp"), {13, 17});
			//if (!isWhite) compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2Hor.bmp"), {13, 17});
			if (isWhite) 
			{
				arrow2.rect.pos.x = rect.getSize().x - (1) * arrow1.rect.getSize().y;
				compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2WhiteVert.bmp"), {16, 16});
			}
		}
		else
		{
			arrow1.rect.pos.x = 0;
			arrow1.rect.pos.y = 0;
			arrow1.rect.finishPos.x = rect.getSize().x;
			arrow1.rect.finishPos.y = (0.76) * arrow1.rect.getSize().x;
			if (!isWhite) compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1.bmp"), {17, 13});
			if (isWhite) 
			{
				arrow1.rect.finishPos.y = (1) * arrow1.rect.getSize().x;
				compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1WhiteVert.bmp"), {16, 16});
			}

			arrow2.rect.pos.x = 0;
			arrow2.rect.pos.y = rect.getSize().y - (0.76) * arrow1.rect.getSize().x;
			arrow2.rect.finishPos.x = rect.getSize().x;
			arrow2.rect.finishPos.y = rect.getSize().y;
			if (!isWhite) compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2.bmp"), {17, 13});
			if (isWhite) 
			{
				arrow2.rect.pos.y = rect.getSize().y - (1) * arrow1.rect.getSize().x;
				compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2WhiteVert.bmp"), {16, 16});
			}
		}

		
		
		if (horizontalMode) 
		{
			compressImage (dc, {rect.getSize ().x - arrow1.rect.getSize().x - arrow2.rect.getSize().x, rect.getSize ().y}, txLoadImage ("SliderBackGroundHor.bmp"), {948, 13});
		}
		else
		{
			if (!isWhite) compressImage (dc, {rect.getSize ().x, rect.getSize ().y - arrow1.rect.getSize().y - arrow2.rect.getSize().y}, txLoadImage ("SliderBackGroundVert.bmp"), {13, 948});
			if (isWhite)  compressImage (dc, {rect.getSize ().x, rect.getSize ().y - arrow1.rect.getSize().y - arrow2.rect.getSize().y}, txLoadImage ("SliderBackGroundWhite.bmp"), {16, 870});
		}
	
		if (horizontalMode)
		{
			sliderQuadrate.rect.pos.y       = 0;
			sliderQuadrate.rect.finishPos.y = rect.getSize ().y;
		}
		else
		{
			if (isWhite)
			{
				compressImage (upSideOfQuadrateSlider.dc, {rect.getSize().x - 2, floor ((rect.getSize().x / 14) * 3)}, txLoadImage ("UpSideOfWhiteQuadrateSlider.bmp"), {14, 3});
				upSideOfQuadrateSlider.rect.pos.x = 1;
				upSideOfQuadrateSlider.rect.finishPos.x = rect.getSize().x - 1;
				upSideOfQuadrateSlider.rect.size.y = floor ((rect.getSize().x / 14) * 3); 

				compressImage (bottomSideOfQuadrateSlider.dc, {rect.getSize().x - 2, floor ((rect.getSize().x / 14) * 3)}, txLoadImage ("BottomSideOfWhiteQuadrateSlider.bmp"), {14, 3});
				bottomSideOfQuadrateSlider.rect.pos.x = 1;
				bottomSideOfQuadrateSlider.rect.finishPos.x = rect.getSize().x - 1;
				bottomSideOfQuadrateSlider.rect.size.y = floor ((rect.getSize().x / 14) * 3); 
			}
			sliderQuadrate.rect.pos.x       = 1;
			sliderQuadrate.rect.finishPos.x = rect.getSize().x - 1;
		}

		if (horizontalMode)
		{
			axis = (rect.getSize().x - arrow2.rect.getSize ().x) - (arrow1.rect.getSize ().x) - rect.getSize().x * quadrateScale;
		}
		else
		{
			axis = (rect.getSize().y - arrow2.rect.getSize ().y) - (arrow1.rect.getSize ().y) - rect.getSize().y * quadrateScale - bottomSideOfQuadrateSlider.rect.size.y;
		}




		if (horizontalMode) 
		{
			compressImage (sliderQuadrate.dc, {rect.getSize ().x * _quadrateScale, sliderQuadrate.rect.getSize().y}, txLoadImage ("SliderRect.bmp"), {316, 12});
			sliderQuadrate.finalDC = txCreateCompatibleDC (rect.getSize ().x * _quadrateScale, sliderQuadrate.rect.getSize().y);
		}
		else
		{

			if (!isWhite) compressImage (sliderQuadrate.dc, {sliderQuadrate.rect.getSize().x, rect.getSize().y *  _quadrateScale}, txLoadImage ("VerticalSliderRect.bmp"), {12, 316});
			if (isWhite) compressImage (sliderQuadrate.dc, {sliderQuadrate.rect.getSize().x, ceil (rect.getSize().y *  _quadrateScale) - bottomSideOfQuadrateSlider.rect.size.y}, txLoadImage ("WhiteSliderRect.bmp"), {14, 299});
			sliderQuadrate.finalDC = txCreateCompatibleDC (sliderQuadrate.rect.getSize().x, rect.getSize().y *  _quadrateScale);
		}

		arrow1.finalDC = txCreateCompatibleDC (arrow1.rect.getSize().x, arrow1.rect.getSize().y);
		arrow2.finalDC = txCreateCompatibleDC (arrow2.rect.getSize().x, arrow2.rect.getSize().y);

		//printBlt (arrow1.finalDC);
		//printBlt (arrow2.finalDC);
		
		//upSideOfQuadrateSlider.finalDC = txCreateCompatibleDC (upSideOfQuadrateSlider.rect.getSize().x, upSideOfQuadrateSlider.rect.getSize().y);
		//bottomSideOfQuadrateSlider.finalDC = txCreateCompatibleDC (bottomSideOfQuadrateSlider.rect.getSize().x, bottomSideOfQuadrateSlider.rect.getSize().y);

	}


	virtual void draw () override;
	virtual void onClick () override;

	void deleteDC ();
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
	Slider brightnessSlider;
	Slider incomeBrightnessSlider;
	Window closeButton;
	Window grafic;
	Window confirmButton;
	double graficScale;
	bool *confirmBrightness;
	//double copyOfBrightness;

	BrightnessButton (Rect _mainRect, Rect _graficRect, Rect _sliderRect, Rect _incomeBrightnessSliderRect, Rect _confirmButton, Rect _closeButtonRect = {}, bool *_canvasBrightness = NULL) :
		Manager (_mainRect, 5, true, NULL, {.pos = {0, 0}, .finishPos = {_mainRect.getSize ().x, _mainRect.getSize ().y * 0.1}}),
		grafic (_graficRect),
		brightnessSlider (_sliderRect, &Brightness, 0.18, 0, 255, false, true),
		confirmButton (_confirmButton),
		closeButton (_closeButtonRect),
		confirmBrightness (_canvasBrightness),
		incomeBrightnessSlider (_incomeBrightnessSliderRect, &IncomeBrightness, 0.18, 1, 255, false, true)
		{
			brightnessSlider.manager = this;
			incomeBrightnessSlider.manager = this;
			closeButton.manager = this;
			brightnessSlider.manager = this;
			confirmButton.manager = this;
			graficScale = (grafic.rect.getSize().y / grafic.rect.getSize().x);	

			compressImage (dc, {rect.getSize ().x, rect.getSize ().y}, txLoadImage ("Brightness.bmp"), {444, 361});
		}


	virtual void draw () override;
	virtual void onClick () override;
};

bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;
const Vector SCREENSIZE = {1000, 1000};


void RECTangle (const Rect rect, HDC dc = txDC ());
void engine (Manager *manager);
void shiftArrBack    (char arr[], int length);
void shiftArrForward (char arr[], int length);
bool checkDeltaTime (int lastTimeClicked);
void printfDCS (const char *str = "");
void createNumChanger (Manager *menu, NumChange **numchange, Canvas *mainCanvas);
void checkTextLen (int currentNum, int previousNum, int *textLen, int *cursorPosition);
void compressImage (HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize);



int main ()
{
	_txWindowStyle &= ~WS_CAPTION;
	txCreateWindow (SCREENSIZE.x, SCREENSIZE.y);
	txSelectFont ("Arial", 28, 10);

	Manager *manager = new Manager({.pos = {0, 0}, .finishPos = {1000, 1000}}, 7, true, NULL);

	TestPhoto = txLoadImage ("TestPhoto.bmp");

	//Canvas *mainCanvas = new Canvas({.pos = {0, 0}, .finishPos = {500, 500}});
	//manager->addWindow (mainCanvas);

	HDC addNewCanvasDC;
	compressImage (addNewCanvasDC, {50, 50}, txLoadImage ("addNewCanvas.bmp"), {225, 225});
	CanvasManager *canvasManager = new CanvasManager ({.pos = {0, 0}, .finishPos = {1000, 1000}}, addNewCanvasDC);
	manager->addWindow (canvasManager);

	ToolsPalette *toolsPallete = new ToolsPalette({.pos = {0, 100}, .finishPos = {50, 300}}, 4);
	manager->addWindow (toolsPallete);

		Window* line = new Window({ .pos = {0, 0}, .finishPos = {50, 50} }, TX_WHITE, txLoadImage("Line.bmp"));
		toolsPallete->addWindow(line);

		Window *gummi = new Window ({ .pos = {0, 50}, .finishPos = {50, 100}}, TX_WHITE, txLoadImage ("Gummi.bmp"));
		toolsPallete->addWindow (gummi); 

		Window *vignette = new Window ({ .pos = {0, 100}, .finishPos = {50, 150} }, TX_WHITE, txLoadImage ("Vignette.bmp"));
		toolsPallete->addWindow (vignette);

		Window *pen = new Window ({ .pos = {0, 150}, .finishPos = {50, 200}}, TX_WHITE, txLoadImage("Pen.bmp"));
		toolsPallete->addWindow (pen);


	Manager *menu = new Manager({.pos = {588, 0}, .finishPos = {1000, 300}}, 3, true, txLoadImage ("HUD-3.bmp"), {.pos = {0, 0}, .finishPos = {412, 50}});
	manager->addWindow (menu);

	Manager *colorManager = new Manager({.pos = {10, 180}, .finishPos = {170, 220}}, 3, true, NULL, {}, RGB (26, 29, 29));
		menu->addWindow (colorManager);

			ColorButton *redColor = new ColorButton({.pos = {0, 0}, .finishPos = {40, 40}}, RGB (255, 0, 0), txLoadImage ("RedButton.bmp"));
			colorManager->addWindow(redColor);
	
			ColorButton *blueColor = new ColorButton({.pos = {60, 0}, .finishPos = {100, 40}}, RGB (0, 0, 255), txLoadImage ("BlueButton.bmp"));
			colorManager->addWindow(blueColor);

			ColorButton *greenColor = new ColorButton({.pos = {120, 0}, .finishPos = {160, 40}}, RGB (0, 255, 0), txLoadImage ("GreenButton.bmp"));
			colorManager->addWindow(greenColor);

		OpenManager *openManager = new OpenManager({.pos = {15, 135}, .finishPos = {36, 153}}, TX_WHITE, colorManager, txLoadImage ("OpenColorButton.bmp"));
		menu->addWindow (openManager);	

	
		/*
	Slider *xCoordinat = new Slider (   { .pos = {0, 980}, .finishPos = {1000, 1000} }, 
										&mainCanvas->canvasCoordinats.x, 
										0.3,
										0, 1000);	   

	
	Slider *yCoordinat = new Slider (   { .pos = {980, 50}, .finishPos = {1000, 950} }, 
										&mainCanvas->canvasCoordinats.y,
										0.3,
										0, 1000,
										false);	   
										*/


	//CleanButton *cleanButton = new CleanButton({.pos = {10, 90}, .finishPos = {94, 121}}, TX_WHITE, mainCanvas, txLoadImage ("CleanButton.bmp"));
	//menu->addWindow (cleanButton);
	//
	CloseButton *closeButton = new CloseButton({.pos = {0, 0}, .finishPos = {50, 50}}, TX_RED, txLoadImage ("CloseButton.bmp"));
	manager->addWindow (closeButton);




	/*
	BrightnessButton *brightnessButton = new BrightnessButton ({.pos = {300, 300}, .finishPos = {744, 661}}, {.pos = {55, 64}, .finishPos = {312, 318}},
															   {.pos = {322, 64}, .finishPos = {338, 318}}, {.pos = {17, 64}, .finishPos = {33, 318}},
															   {.pos = {357, 63}, .finishPos = {436, 80}},  
															   {.pos = {357, 90}, .finishPos = {436, 100}},
																&mainCanvas->confirmBrightness);	
																*/
	//manager->addWindow (brightnessButton);

	//OpenManager *brightnessButtonOpen = new OpenManager ({.pos = {500, 0}, .finishPos = {550, 50}}, TX_WHITE, brightnessButton);
	//manager->addWindow (brightnessButtonOpen);


	
	txBegin ();



	engine (manager);

	txEnd ();

	redColor->deleteButton ();
	blueColor->deleteButton ();
	blueColor->deleteButton ();

	manager->deleteButton ();
	menu->deleteButton ();
	colorManager->deleteButton ();
	closeButton->deleteButton ();

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



void BrightnessButton::draw ()
{
	if (!advancedMode) return;

	controlHandle ();

	if (dc) copyOnDC (0, 0, dc);
	brightnessSlider.draw ();
	txBitBlt (finalDC, brightnessSlider.rect.pos.x, brightnessSlider.rect.pos.y, 0, 0, brightnessSlider.finalDC);

	incomeBrightnessSlider.draw ();
	txBitBlt (finalDC, incomeBrightnessSlider.rect.pos.x, incomeBrightnessSlider.rect.pos.y, 0, 0, incomeBrightnessSlider.finalDC);
	//txBitBlt (incomeBrightnessSlider.rect.pos.x, incomeBrightnessSlider.rect.pos.y, incomeBrightnessSlider.finalDC);
	txBitBlt (300, 300, incomeBrightnessSlider.finalDC);
	txSetFillColor (TX_RED);
	txRectangle (300, 300, 400, 400);

	txSetAllColors (BackgroundColor, finalDC);
	txLine (grafic.rect.pos.x, grafic.rect.pos.y + grafic.rect.getSize ().y * (IncomeBrightness / 255),
			grafic.rect.finishPos.x, grafic.rect.pos.y + grafic.rect.getSize ().y * (Brightness / 255), finalDC);
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

	if (brightnessSlider.getAbsRect().inRect (mx, my) && !isClicked) brightnessSlider.onClick ();		
	if (incomeBrightnessSlider.getAbsRect().inRect (mx, my) && !isClicked) incomeBrightnessSlider.onClick ();		
	if (closeButton.getAbsRect().inRect (mx, my) && !isClicked) advancedMode = false;
	if (confirmButton.getAbsRect().inRect (mx, my) && !isClicked) 
	{
		*confirmBrightness = true;
		brightnessSlider.maxNum = Brightness;
		incomeBrightnessSlider.maxNum = IncomeBrightness;
	}
}

void Slider::deleteDC ()
{
	txDeleteDC (sliderQuadrate.dc);
	txDeleteDC (arrow1.dc);
	txDeleteDC (arrow2.dc);
	txDeleteDC (dc);
	txDeleteDC (upSideOfQuadrateSlider.dc);
	txDeleteDC (bottomSideOfQuadrateSlider.dc);
}

void compressImage (HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize)
{
	if (newSize.x <= 0 || newSize.y <= 0)
		newSize.print ();

	newDC = txCreateCompatibleDC (newSize.x, newSize.y);

	assert (oldDC);
	assert (StretchBlt (newDC, 0, 0, newSize.x, newSize.y, oldDC, 0, 0, oldSize.x, oldSize.y, SRCCOPY));
	txDeleteDC (oldDC);
}

void Slider::draw ()
{
	kScale = axis / (maxNum - minNum);

	int mx = txMouseX ();
	int my = txMouseY ();
	Vector mousePos =  {mx, my};

	txSetAllColors (TX_WHITE, finalDC);

	if (horizontalMode)  txBitBlt (finalDC, arrow1.rect.getSize ().x, 0, rect.getSize ().x, rect.getSize ().y, dc);
	if (!horizontalMode) txBitBlt (finalDC, 0, arrow1.rect.getSize ().y, rect.getSize ().x, rect.getSize ().y, dc);

	arrow1.draw ();
	txBitBlt (finalDC, arrow1.rect.pos.x, arrow1.rect.pos.y, rect.getSize ().x, rect.getSize ().y, arrow1.finalDC);

	arrow2.draw ();
	txBitBlt (finalDC, arrow2.rect.pos.x, arrow2.rect.pos.y, rect.getSize ().x, rect.getSize ().y, arrow2.finalDC);

	//upSideOfQuadrateSlider.draw ();
	//txBitBlt (finalDC, upSideOfQuadrateSlider.rect.pos.x, upSideOfQuadrateSlider.rect.pos.y, rect.getSize ().x, rect.getSize ().y, upSideOfQuadrateSlider.finalDC);

	//bottomSideOfQuadrateSlider.draw ();
	//txBitBlt (finalDC, bottomSideOfQuadrateSlider.rect.pos.x, bottomSideOfQuadrateSlider.rect.pos.y, rect.getSize ().x, rect.getSize ().y, bottomSideOfQuadrateSlider.finalDC);

	if (txMouseButtons () != 1) sliderQuadrate.isClicked = false;

	sliderQuadrate.draw ();
	copyOnDC (sliderQuadrate.rect.pos.x, sliderQuadrate.rect.pos.y, sliderQuadrate.dc);
	//txBitBlt (finalDC, arrow2.rect.pos.x, arrow2.rect.pos.y, rect.getSize ().x, rect.getSize ().y, sliderQuadrate.dc);


	if (sliderQuadrate.isClicked)
	{
		if (horizontalMode)  *num = tempNum + ( (mousePos.x - cursorStartPosition.x) / (kScale));
		if (!horizontalMode) *num = tempNum + ( (mousePos.y - cursorStartPosition.y) / (kScale)); 

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
		upSideOfQuadrateSlider.rect.pos.y = kScale * (*num - minNum) + arrow1.rect.finishPos.y;
		upSideOfQuadrateSlider.rect.finishPos.y = kScale * (*num - minNum) + arrow1.rect.finishPos.y + upSideOfQuadrateSlider.rect.size.y;

		sliderQuadrate.rect.pos.y		= upSideOfQuadrateSlider.rect.finishPos.y;
		sliderQuadrate.rect.finishPos.y = quadrateScale * rect.getSize ().y + upSideOfQuadrateSlider.rect.finishPos.y - bottomSideOfQuadrateSlider.rect.size.y;

		bottomSideOfQuadrateSlider.rect.pos.y = sliderQuadrate.rect.finishPos.y;
		bottomSideOfQuadrateSlider.rect.finishPos.y = bottomSideOfQuadrateSlider.rect.pos.y + bottomSideOfQuadrateSlider.rect.size.y;

	}

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
								txLoadImage ("SwitchButton.bmp"), 
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
	controlHandle();

	if (dc) txBitBlt(finalDC, 0, 0, 0, 0, dc);

	for (int i = 0; i < newButtonNum; i++)
	{
		pointers[i]->draw();
		if (pointers[i]->advancedMode) txBitBlt(finalDC, pointers[i]->rect.pos.x, pointers[i]->rect.pos.y, pointers[i]->rect.finishPos.x, pointers[i]->rect.finishPos.y, pointers[i]->finalDC);
		if (lastSelected == i)
		{
			txSetAllColors(TX_WHITE, finalDC);
			txRectangle(pointers[i]->rect.pos.x, pointers[i]->rect.pos.y, pointers[i]->rect.pos.x + pointers[i]->rect.getSize().x * 0.1, pointers[i]->rect.pos.y + pointers[i]->rect.getSize().y * 0.1, finalDC);
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
}

void ToolsPalette::onClick ()
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

void Manager::draw ()
{
	controlHandle ();

	if (dc) txBitBlt (finalDC, 0, 0, 0, 0, dc);


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
		activeWindow = NULL;
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

Vector Window::getAbsCoordinats ()
{
	Vector coordinats = {};

	Manager *copyOfManager = manager;

	coordinats += rect.pos;

	for (;;)
	{
		if (!copyOfManager) break;

		coordinats = coordinats + copyOfManager->rect.pos;
		copyOfManager = copyOfManager->manager;

	}

	return coordinats;
}

Rect Window::getAbsRect ()
{
	Rect coordinats = {};

	Manager *copyOfManager = manager;

	coordinats.pos += rect.pos;

	for (;;)
	{
		if (!copyOfManager) break;

		coordinats.pos += copyOfManager->rect.pos; 
		copyOfManager = copyOfManager->manager;
	}

	coordinats.finishPos = rect.finishPos + (coordinats.pos - rect.pos);

	return coordinats;
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

void txSetAllColors (COLORREF color, HDC dc /*= txDc ()*/)
{
	txSetFillColor (color, dc); 
	txSetColor (color, 1, dc);
}

void RECTangle (const Rect rect, HDC dc /* = txDc ()*/)
{
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}


void TimeButton::draw ()
{
	txSetAllColors (BackgroundColor);
	time_t t = time (NULL);
	t = t % (24 * 3600);

	char newStr[50] = {};
	int hours =	t / 3600;
	int minutes = t / 60 - hours * 60;
	int second  = t - hours * 3600 - minutes * 60;

	sprintf (newStr, "%d:%02d:%02d", hours + 3, minutes, second);

	txSetTextAlign (TA_LEFT);
	txSetAllColors (TX_WHITE);
	txTextOut (rect.pos.x, rect.pos.y, newStr);

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

void Window::draw ()
{
	$s
	if (finalDC) txSetAllColors (color, finalDC);
	if (finalDC) txRectangle (0, 0, rect.getSize ().x, rect.getSize ().y, finalDC);


	if (finalDC) txSetAllColors (TX_BLACK, finalDC);
	txSetTextAlign (TA_CENTER, finalDC);
	txTextOut (rect.getSize ().x / 2, rect.getSize().y / 10, text, finalDC);


	if (dc) 
	{
		txBitBlt (finalDC, 0, 0, rect.size.x, rect.size.y, dc);	
	}
}

void Window::deleteButton ()
{
	if (dc) txDeleteDC (dc);
	if (finalDC) txDeleteDC (dc);
}

void Window::resizeDC (Vector newSize)
{
	txDeleteDC (finalDC);
}


void CanvasManager::draw ()
{
	txSetAllColors (BackgroundColor, finalDC);
	txRectangle (0, 0, 3000, 3000, finalDC);


	if (dc) txBitBlt (finalDC, 0, 0, 0, 0, dc);

	if (newCanvas.isClicked)
	{
		addWindow(new Canvas({ .pos = {0, newCanvas.rect.finishPos.y}, .finishPos = {500, newCanvas.rect.finishPos.y + 500} }, closeCanvasButton));
		newCanvas.isClicked = false;
	}
	
	newCanvas.draw();
	txBitBlt(finalDC, newCanvas.rect.pos.x, newCanvas.rect.pos.y, 0, 0, newCanvas.finalDC);

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
		activeWindow = NULL;
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

		if (newCanvas.getAbsRect().inRect (mx, my) && !newCanvas.isClicked)
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


void Canvas::draw ()
{
	assert (canvas);
	txSetAllColors (BackgroundColor, finalDC);
	txRectangle (0, 0, 3000, 3000, finalDC);

	for (int x = 0; x < DCMAXSIZE; x++)
	{
		for (int y = 0; y < DCMAXSIZE; y++)
		{
			RGBQUAD pixel = canvasArr [int (x + canvasCoordinats.x + (int) ((DCMAXSIZE - y - canvasCoordinats.y)) * (DCMAXSIZE -1))];


			pixel.rgbRed = pixel.rgbRed * ((255 - Brightness) - (255 - IncomeBrightness)) / 255.0 + 255 - IncomeBrightness;
			pixel.rgbGreen = pixel.rgbGreen * ((255 - Brightness) - (255 - IncomeBrightness)) / 255.0 + 255 - IncomeBrightness;
 			pixel.rgbBlue = pixel.rgbBlue * ((255 - Brightness) - (255 - IncomeBrightness)) / 255.0 + 255 - IncomeBrightness;

			finalDCArr[(int) (x + (DCMAXSIZE - y) * (DCMAXSIZE - 1))] = pixel;

			if (confirmBrightness) 
			{
				canvasArr [int (x + canvasCoordinats.x + (int) ((y + SCREENSIZE.y - canvasCoordinats.y)) * DCMAXSIZE)] = pixel;
				confirmBrightness = false;
			}
		}
	}


	if (manager->activeWindow != this) wasClicked = false;


	if (txMouseButtons () == 2 && wasClicked)
	{
		txSetAllColors (DrawColor, canvas);
		txSetColor (DrawColor, lineThickness, canvas);
		txLine (lastClick.x + canvasCoordinats.x, lastClick.y + canvasCoordinats.y, txMouseX () - getAbsCoordinats().x + canvasCoordinats.x, txMouseY () - getAbsCoordinats().y + canvasCoordinats.y, canvas);
		wasClicked = false;
	}

	if (wasClicked && manager->activeWindow != this) wasClicked = false;

	if (wasClicked)
	{
		txSetAllColors (DrawColor, finalDC);
		txSetColor (DrawColor, lineThickness, finalDC);
		txLine (lastClick.x, lastClick.y, txMouseX () - getAbsCoordinats().x, txMouseY () - getAbsCoordinats().y, finalDC);
	}

	if (clearBackground)
	{
		txSetAllColors (BackgroundColor, canvas);
		txRectangle (0, 0, canvasSize.x, canvasSize.y, canvas);
		clearBackground = false;
	}

	controlHandle();
	drawOnFinalDC(handle);
	controlSize();
	

	txSetAllColors(TX_WHITE, finalDC);
	txRectangle (0, 0, rect.getSize().x * 0.01 + 1, rect.getSize().y, finalDC);
	txRectangle (0, rect.getSize().y * 0.99 - 1, rect.getSize().x, rect.getSize().y, finalDC);
	txRectangle(rect.getSize().x * 0.99 - 1, rect.getSize().y * 0.99 - 1l, rect.getSize().x, 0, finalDC);
	drawOnFinalDC(closeCanvas);
}

void Canvas::controlSize()
{
	if (isResizing)
	{
		rect.finishPos.x += txMouseX() - startResizingCursor.x;
		rect.finishPos.y += txMouseY() - startResizingCursor.y;
		startResizingCursor = { txMouseX(), txMouseY() };
	}
	if (txMouseButtons() != 1) isResizing = false;
}

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

void Canvas::onClick ()
{
	txSetAllColors ( drawColor);
	lastClick = { .x = txMouseX() - getAbsCoordinats().x, .y = txMouseY() - getAbsCoordinats().y };

	if (!(isClicked) && manager->activeWindow == this)
	{
		int mx = txMouseX();
		int my = txMouseY();

		if (closeCanvas.getAbsRect().inRect(mx, my))
		{
			advancedMode = false;
			return;
		}

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

		if (DrawingMode == 1) wasClicked = true;
	}

	if (DrawingMode == 2)
	{
		txSetAllColors(BackgroundColor, canvas);
		txEllipse(lastClick.x - 20, lastClick.y - 20, lastClick.x + 20, lastClick.y + 20, canvas);
	}

	if (DrawingMode == 3)
	{
		RGBQUAD pixel = canvasArr[(int)(lastClick.x) + (int) ((canvasSize.y - lastClick.y) * canvasSize.x)];
		DrawColor = RGB (pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue);
	}
	if (DrawingMode == 4)
	{
		txSetAllColors (DrawColor, canvas);
		txEllipse (lastClick.x - 5, lastClick.y - 5, lastClick.x + 5, lastClick.y + 5, canvas);
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