
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"
#include <cmath>



const int DELTACLOCKTIME = 100;
double Brightness = 255;
double IncomeBrightness = 255;


HDC TestPhoto;


#define printBlt(image)         \
{                               \
    assert (image);				\
	txBitBlt (0, 0, image);     \
    _getch ();                  \
    txClear ();                 \
}

void compressImage (HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize);

struct Manager;

struct Window
{
	Rect rect;
	COLORREF color;
	const char *text;
	bool isClicked;
	HDC dc;
	Manager *manager;
	bool advancedMode;

	virtual void draw ();
	virtual void onClick () {};

	Window (Rect _rect, COLORREF _color = TX_WHITE, HDC _dc = NULL, Manager *_manager = NULL, const char *_text = "", bool _advancedMode = true) :
		rect (_rect),
		color(_color),
		manager (_manager),
		text (_text), 
		isClicked (false), 
		dc (_dc),
		advancedMode (_advancedMode)
	{}
};

struct Canvas : Window
{
	HDC canvas;
	double lineThickness = 3;
	COLORREF drawColor = TX_RED;
	bool clearBackground = true;
	Vector canvasCoordinats;
	Vector canvasSize;
	RGBQUAD *pixelsArr;
	bool confirmBrightness;

	bool wasClicked = false;
	Vector lastClick = {};

	Canvas (Rect _rect, COLORREF _color, Vector _canvasSize) : 
		Window (_rect, _color),
		canvasCoordinats ({}),
		canvasSize (_canvasSize),
		confirmBrightness (false)
	{
		canvas = txCreateDIBSection (_canvasSize.x, _canvasSize.y, &pixelsArr);
	}

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
	Canvas *mainCanvas;

	ColorButton(Rect _rect, COLORREF _color, Canvas *_mainCanvas, HDC _dc) :
		Window (_rect, _color, _dc),
		mainCanvas (_mainCanvas)
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

struct Manager : Window
{
	int length;
	Window **pointers = NULL;
	int newButtonNum;
	Window *activeWindow;
	bool mainWindow;

	Manager (Rect _rect, int _length, bool _advancedMode = true, HDC _dc = NULL) : 
		Window (_rect, TX_WHITE, _dc, NULL, "", _advancedMode),
		length (_length),
		pointers (new Window*[length]{}),
		newButtonNum (0),
		activeWindow (NULL), 
		mainWindow (false)
	{}

	bool addWindow (Window *window);
	Window *getActiveWindow ();

	virtual void draw () override;
	virtual void onClick () override;
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

struct Slider : Window 
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
		Window (_mainRect, TX_BLACK),
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

		if (horizontalMode)
		{
			arrow1.rect.pos.x = rect.pos.x;
			arrow1.rect.pos.y = rect.pos.y;
			arrow1.rect.finishPos.x = rect.pos.x + (1.3) * (rect.finishPos.y - arrow1.rect.pos.y);
			arrow1.rect.finishPos.y = rect.finishPos.y;
			//compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1Hor.bmp"), {13, 17});
			if (!isWhite) compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1Hor.bmp"), {13, 17});
			if (isWhite) 
			{
				arrow1.rect.finishPos.x = rect.pos.x + (1) * arrow1.rect.getSize().y;
				compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1WhiteVert.bmp"), {16, 16});
			}


			arrow2.rect.pos.x = rect.finishPos.x - (1.3) * (rect.finishPos.y - arrow1.rect.pos.y);
			arrow2.rect.pos.y = rect.pos.y;
			arrow2.rect.finishPos.x = rect.finishPos.x;
			arrow2.rect.finishPos.y = rect.finishPos.y;
			//compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2Hor.bmp"), {13, 17});
			if (!isWhite) compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2Hor.bmp"), {13, 17});
			if (isWhite) 
			{
				arrow2.rect.pos.x = rect.finishPos.x - (1) * arrow1.rect.getSize().y;
				compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2WhiteVert.bmp"), {16, 16});
			}
		}
		else
		{
			arrow1.rect.pos.x = rect.pos.x;
			arrow1.rect.pos.y = rect.pos.y;
			arrow1.rect.finishPos.x = rect.finishPos.x;
			arrow1.rect.finishPos.y = rect.pos.y + (0.76) * arrow1.rect.getSize().x;
			if (!isWhite) compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1.bmp"), {17, 13});
			if (isWhite) 
			{
				arrow1.rect.finishPos.y = rect.pos.y + (1) * arrow1.rect.getSize().x;
				compressImage (arrow1.dc, {arrow1.rect.getSize().x, arrow1.rect.getSize().y}, txLoadImage ("arrow1WhiteVert.bmp"), {16, 16});
			}

			arrow2.rect.pos.x = rect.pos.x;
			arrow2.rect.pos.y = rect.finishPos.y - (0.76) * arrow1.rect.getSize().x;
			arrow2.rect.finishPos.x = rect.finishPos.x;
			arrow2.rect.finishPos.y = rect.finishPos.y;
			if (!isWhite) compressImage (arrow2.dc, {arrow2.rect.getSize().x, arrow2.rect.getSize().y}, txLoadImage ("arrow2.bmp"), {17, 13});
			if (isWhite) 
			{
				arrow2.rect.pos.y = rect.finishPos.y - (1) * arrow1.rect.getSize().x;
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
			sliderQuadrate.rect.pos.y       = rect.pos.y;
			sliderQuadrate.rect.finishPos.y = rect.finishPos.y;
		}
		else
		{
			if (isWhite)
			{
				compressImage (upSideOfQuadrateSlider.dc, {rect.getSize().x - 2, floor ((rect.getSize().x / 14) * 3)}, txLoadImage ("UpSideOfWhiteQuadrateSlider.bmp"), {14, 3});
				upSideOfQuadrateSlider.rect.pos.x = rect.pos.x + 1;
				upSideOfQuadrateSlider.rect.finishPos.x = rect.finishPos.x - 1;
				upSideOfQuadrateSlider.rect.size.y = floor ((rect.getSize().x / 14) * 3); 

				compressImage (bottomSideOfQuadrateSlider.dc, {rect.getSize().x - 2, floor ((rect.getSize().x / 14) * 3)}, txLoadImage ("BottomSideOfWhiteQuadrateSlider.bmp"), {14, 3});
				bottomSideOfQuadrateSlider.rect.pos.x = rect.pos.x + 1;
				bottomSideOfQuadrateSlider.rect.finishPos.x = rect.finishPos.x - 1;
				bottomSideOfQuadrateSlider.rect.size.y = floor ((rect.getSize().x / 14) * 3); 
			}
			sliderQuadrate.rect.pos.x       = rect.pos.x + 1;
			sliderQuadrate.rect.finishPos.x = rect.finishPos.x - 1;
		}

		if (horizontalMode)
		{
			axis = (rect.finishPos.x - arrow2.rect.getSize ().x) - (rect.pos.x + arrow1.rect.getSize ().x) - rect.getSize().x * quadrateScale;
		}
		else
		{
			axis = (rect.finishPos.y - arrow2.rect.getSize ().y) - (rect.pos.y + arrow1.rect.getSize ().y) - rect.getSize().y * quadrateScale - bottomSideOfQuadrateSlider.rect.size.y;
		}




		if (horizontalMode) 
		{
			compressImage (sliderQuadrate.dc, {rect.getSize ().x * _quadrateScale, sliderQuadrate.rect.getSize().y}, txLoadImage ("SliderRect.bmp"), {316, 12});
		}
		else
		{

			if (!isWhite) compressImage (sliderQuadrate.dc, {sliderQuadrate.rect.getSize().x, rect.getSize().y *  _quadrateScale}, txLoadImage ("VerticalSliderRect.bmp"), {12, 316});
			if (isWhite) compressImage (sliderQuadrate.dc, {sliderQuadrate.rect.getSize().x, ceil (rect.getSize().y *  _quadrateScale) - bottomSideOfQuadrateSlider.rect.size.y}, txLoadImage ("WhiteSliderRect.bmp"), {14, 299});
		}
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
		Manager (_mainRect, 5, false),
		grafic (_graficRect),
		brightnessSlider (_sliderRect, &Brightness, 0.18, 0, 255, false, true),
		confirmButton (_confirmButton),
		closeButton (_closeButtonRect),
		confirmBrightness (_canvasBrightness),
		incomeBrightnessSlider (_incomeBrightnessSliderRect, &IncomeBrightness, 0.18, 1, 255, false, true)
		{
			graficScale = (grafic.rect.getSize().y / grafic.rect.getSize().x);	

			compressImage (dc, {rect.getSize ().x, rect.getSize ().y}, txLoadImage ("Brightness.bmp"), {444, 361});
		}


	virtual void draw () override;
	virtual void onClick () override;
};

//typedef void (Window::*funcClickedPtr) (const Window *);

		
//COLORREF CurrColor = TX_RED;
//const int PointersNum = 9;
bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;
const Vector SCREENSIZE = {1000, 1000};


void RECTangle (const Rect rect, HDC dc = txDC ());
void txSetAllColors (COLORREF color, HDC dc = txDC ());
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
	//_txWindowStyle &= ~WS_CAPTION;
	txCreateWindow (SCREENSIZE.x, SCREENSIZE.y);
	txSelectFont ("Arial", 28, 10);

	TestPhoto = txLoadImage ("TestPhoto.bmp");

	Manager *manager = new Manager({.pos = {0, 0}, .finishPos = {1000, 1000}}, 13, true);

	Canvas *mainCanvas = new Canvas({.pos = {0, 0}, .finishPos = {1000, 1000}}, TX_BLACK, {2000, 2000});
	manager->addWindow (mainCanvas);

	Manager *menu = new Manager({.pos = {0, 0}, .finishPos = {412, 300}}, 5, true, txLoadImage ("HUD-3.bmp"));
	//manager->addWindow (menu);

		Manager *colorManager = new Manager({.pos = {10, 180}, .finishPos = {170, 220}}, 3, false);
		menu->addWindow (colorManager);

			ColorButton *redColor = new ColorButton({.pos = {10, 180}, .finishPos = {50, 220}}, RGB (255, 0, 0), mainCanvas, txLoadImage ("RedButton.bmp"));
			colorManager->addWindow(redColor);
	
			ColorButton *blueColor = new ColorButton({.pos = {70, 180}, .finishPos = {110, 220}}, RGB (0, 0, 255), mainCanvas, txLoadImage ("BlueButton.bmp"));
			colorManager->addWindow(blueColor);

			ColorButton *greenColor = new ColorButton({.pos = {130, 180}, .finishPos = {170, 220}}, RGB (0, 255, 0), mainCanvas, txLoadImage ("GreenButton.bmp"));
			colorManager->addWindow(greenColor);

		OpenManager *openManager = new OpenManager({.pos = {15, 135}, .finishPos = {36, 153}}, TX_WHITE, colorManager, txLoadImage ("OpenColorButton.bmp"));
		menu->addWindow (openManager);

	NumChange *numChange = NULL;
	createNumChanger (menu, &numChange, mainCanvas);	

	Slider *xCoordinat = new Slider (   { .pos = {0, 980}, .finishPos = {1000, 1000} }, 
										&mainCanvas->canvasCoordinats.x, 
										0.3,
										0, 1000);

	
	Slider *yCoordinat = new Slider (   { .pos = {980, 50}, .finishPos = {1000, 950} }, 
										&mainCanvas->canvasCoordinats.y,
										0.3,
										0, 1000,
										false);	  

	manager->addWindow (xCoordinat);
	manager->addWindow (yCoordinat);

	//static int num2 = 3;
	//Slider *slider = new Slider ( {.pos = {70, 60}, .finishPos = {280, 70} }, {.pos = {70, 60}, .finishPos = {90, 70} }, {.pos = {260, 60}, .finishPos = {280, 70} }, (numChange->num), 0, 50, true);
	//Slider *slider = new Slider ( {.pos = {70, 70}, .finishPos = {80, 280} }, {.pos = {70, 260}, .finishPos = {80, 280} }, {.pos = {70, 70}, .finishPos = {80, 90} }, &num2, 0, 50, false);
	//menu->addWindow (slider);

	CleanButton *cleanButton = new CleanButton({.pos = {10, 90}, .finishPos = {94, 121}}, TX_WHITE, mainCanvas, txLoadImage ("CleanButton.bmp"));
	menu->addWindow (cleanButton);

	CloseButton *closeButton = new CloseButton({.pos = {950, 0}, .finishPos = {1000, 50}}, TX_RED, txLoadImage ("CloseButton.bmp"));
	
	manager->addWindow (closeButton);

	TimeButton *timeButton = new TimeButton ({.pos = {835, 0}, .finishPos = {850, 50}});
	menu->addWindow (timeButton);

	BrightnessButton *brightnessButton = new BrightnessButton ({.pos = {300, 300}, .finishPos = {744, 661}}, {.pos = {355, 364}, .finishPos = {612, 618}},
															   {.pos = {622, 364}, .finishPos = {638, 618}}, {.pos = {317, 364}, .finishPos = {333, 618}},
															   {.pos = {657, 363}, .finishPos = {736, 380}},  
															   {.pos = {657, 390}, .finishPos = {736, 400}},
																&mainCanvas->confirmBrightness);
	manager->addWindow (brightnessButton);

	OpenManager *brightnessButtonOpen = new OpenManager ({.pos = {500, 0}, .finishPos = {550, 50}}, TX_WHITE, brightnessButton);
	manager->addWindow (brightnessButtonOpen);
	
	txBegin ();

	engine (manager);

	txEnd ();

	txDeleteDC (mainCanvas->canvas);
	txDeleteDC (redColor->dc);
	txDeleteDC (blueColor->dc);
	txDeleteDC (greenColor->dc);

	txDeleteDC (numChange->plusMinusDC);
	numChange->slider.deleteDC ();
	brightnessButton->brightnessSlider.deleteDC ();
	txDeleteDC (brightnessButton->dc);
	txDeleteDC (brightnessButtonOpen->dc);

	txDeleteDC (cleanButton->dc);
	txDeleteDC (menu->dc); 
	txDeleteDC (openManager->dc);
	txDeleteDC (closeButton->dc);
	txDeleteDC (xCoordinat->dc);
	xCoordinat->deleteDC ();
	yCoordinat->deleteDC ();
	txDisableAutoPause ();

	return 0;

}

void BrightnessButton::draw ()
{
	if (!advancedMode) return;
	txBitBlt (rect.pos.x, rect.pos.y, dc);
	brightnessSlider.draw ();
	incomeBrightnessSlider.draw ();

	txSetColor (TX_BLACK);
	txLine (grafic.rect.pos.x, grafic.rect.pos.y + grafic.rect.getSize ().y * (Brightness / 255),
			grafic.rect.pos.x + grafic.rect.getSize ().x * (IncomeBrightness / 255), grafic.rect.pos.y);
}

void BrightnessButton::onClick ()
{
	if (!advancedMode) return;
	int mx = txMouseX ();
	int my = txMouseY ();

	if (brightnessSlider.rect.inRect (mx, my) && !isClicked) brightnessSlider.onClick ();		
	if (incomeBrightnessSlider.rect.inRect (mx, my) && !isClicked) incomeBrightnessSlider.onClick ();		
	if (closeButton.rect.inRect (mx, my) && !isClicked) advancedMode = false;
	if (confirmButton.rect.inRect (mx, my) && !isClicked) 
	{
		*confirmBrightness = true;
		brightnessSlider.maxNum = Brightness;
	}
	if (confirmButton.rect.inRect (mx, my) && !isClicked) 
	{
		*confirmBrightness = true;
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

	txSetAllColors (TX_WHITE);

	if (horizontalMode)  txBitBlt (rect.pos.x + arrow1.rect.getSize ().x, rect.pos.y, dc);
	if (!horizontalMode) txBitBlt (rect.pos.x, rect.pos.y + arrow1.rect.getSize ().y, dc);

	arrow1.draw ();
	arrow2.draw ();
	upSideOfQuadrateSlider.draw ();
	bottomSideOfQuadrateSlider.draw ();

	if (txMouseButtons () != 1) sliderQuadrate.isClicked = false;

	sliderQuadrate.draw ();

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


	if (sliderQuadrate.rect.inRect (mx, my) && !isClicked)
	{
		cursorStartPosition = {(double) mx, (double) my};
		tempNum = *num;
		sliderQuadrate.isClicked = true;
	}

	if (arrow1.rect.inRect (mx, my) && *num > minNum && !isClicked)
	{
		(*num)--;
	}
	if (arrow2.rect.inRect (mx, my) && *num < maxNum && !isClicked)
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

void Manager::draw ()
{
	if (advancedMode)
	{
		if (dc != NULL) txBitBlt (rect.pos.x, rect.pos.y, dc);
		for (int i = 0; i < newButtonNum; i++)
		{
			pointers[i]->draw();
			if (txMouseButtons () != 1 && pointers[i]->isClicked != false)
			{
				pointers[i]->isClicked = false;
			}
		}
	}

	if (manager->getActiveWindow () != this && mainWindow == false) 
	{
		activeWindow = NULL;
	}
}	

void Manager::onClick ()
{
	bool missClicked = true;

	int mx = txMouseX ();
	int my = txMouseY ();

	if (advancedMode)
	{
		for (int i = newButtonNum - 1; i >= 0; i--)
		{
			
			if (pointers[i]->rect.inRect (mx, my))
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
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	if (dc) txBitBlt (rect.pos.x, rect.pos.y, dc);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		//txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
}

void engine (Manager *manager)
{
	manager->mainWindow = true;

	for (;;)
	{
		manager->draw ();
		if (txMouseButtons () == 1 && manager->rect.inRect (txMouseX (), txMouseY ()))
		{
			manager->onClick ();
			if (!IsRunning) break;
		}
		txSleep (30);

		txSetAllColors (TX_BLACK);
		txRectangle (manager->rect.pos.x, manager->rect.pos.y, manager->rect.finishPos.x, manager->rect.finishPos.y);
		//txClear ();
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
	txSetAllColors (TX_BLACK);
	time_t t = time (NULL);
	t = t % (24 * 3600);

	char newStr[50] = {};
	int hours =	t / 3600;
	int minutes = t / 60 - hours * 60;
	int second  = t - hours * 3600 - minutes * 60;

	//strftime (newStr, 50, "%X", localtime (time));

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

	txBitBlt (rect.pos.x, rect.pos.y, dc);
		

	txSetTextAlign (TA_CENTER);
	txSetAllColors (TX_BLACK);
		
	txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 4, 
					text);
}

void CloseButton::draw()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	txTransparentBlt (rect.pos.x, rect.pos.y, dc);
}

void Window::draw ()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	if (dc != NULL) txBitBlt (rect.pos.x, rect.pos.y, dc);

	txSetAllColors (color);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 5, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
}

void Canvas::draw ()
{
	assert (canvas);
	//txSetAllColors (TX_BLACK);
	//txRectangle (0, 0, 1000, 1000);
	txBitBlt (canvas, 0, 0, 1000, 1000, TestPhoto);

	for (int x = 0; x < 1000; x++)
	{
		for (int y = 0; y < 1000; y++)	
		{
			RGBQUAD pixel = pixelsArr [int (x + canvasCoordinats.x + (int) ((y + SCREENSIZE.y - canvasCoordinats.y)) * canvasSize.x)];

			//pixel.rgbRed = MIN ((pixel.rgbRed / IncomeBrightness)  * 255 + (255 - Brightness), 255);
			//pixel.rgbGreen = MIN ( (pixel.rgbGreen / IncomeBrightness) * 255 + (255 - Brightness), 255);
 			//pixel.rgbBlue = MIN ( (pixel.rgbBlue / IncomeBrightness) * 255 + (255 - Brightness), 255);

			//pixel.rgbRed = MAX ((pixel.rgbRed) + (Brightness - IncomeBrightness), 0);
			//pixel.rgbGreen = MAX ( (pixel.rgbGreen) + (Brightness - IncomeBrightness), 0);
 			//pixel.rgbBlue = MAX ( (pixel.rgbBlue) + (Brightness - IncomeBrightness), 0);

			pixel.rgbRed = MIN ((pixel.rgbRed) * (Brightness - (255 - IncomeBrightness)) / 255.0 + 255 - IncomeBrightness, 255);
			pixel.rgbGreen = MIN (pixel.rgbGreen * (Brightness - (255 - IncomeBrightness)) / 255.0 + 255 - IncomeBrightness, 255);
 			pixel.rgbBlue = MIN (pixel.rgbBlue * (Brightness - (255 - IncomeBrightness)) / 255.0 + 255 - IncomeBrightness, 255);

			//pixel.rgbRed =  (pixel.rgbRed  + (255 - Brightness));
			//pixel.rgbGreen = pixel.rgbGreen  + (255 - Brightness);
 			//pixel.rgbBlue = pixel.rgbBlue  + (255 - Brightness);

			if (confirmBrightness) 
			{
				pixelsArr [int (x + canvasCoordinats.x + (int) ((y + SCREENSIZE.y - canvasCoordinats.y)) * canvasSize.x)] = pixel;
				confirmBrightness = false;
			}

			txVideoMemory ()[(int) (x + (y) * SCREENSIZE.x)] = pixel;
		}
	}

	if (txMouseButtons () == 2 && wasClicked)
	{
		txSetAllColors (drawColor, canvas);
		txSetColor (drawColor, lineThickness, canvas);
		txLine (lastClick.x + canvasCoordinats.x, lastClick.y - rect.pos.y + canvasCoordinats.y, txMouseX () + canvasCoordinats.x, txMouseY () - rect.pos.y + canvasCoordinats.y, canvas); 
		wasClicked = false;
	}

	if (wasClicked)
	{

		drawColor = RGB (MIN ( (txExtractColor (drawColor, TX_RED) / IncomeBrightness * Brightness), 255), MIN (txExtractColor (drawColor, TX_BLUE) / IncomeBrightness * Brightness, 255), MIN (txExtractColor (drawColor, TX_GREEN) / IncomeBrightness * Brightness, 255));
		

		txSetAllColors (drawColor);
		txSetColor (drawColor/ IncomeBrightness * Brightness, lineThickness);
		if (txMouseY () < rect.pos.y)
		{
			txLine (lastClick.x, lastClick.y, txMouseX (), rect.pos.y);	
		}
		else
		{
			txLine (lastClick.x, lastClick.y, txMouseX (), txMouseY ());
		}
	}

	if (clearBackground)
	{
		txSetAllColors (TX_BLACK, canvas);
		txRectangle (0, 0, canvasSize.x, canvasSize.y, canvas);
		clearBackground = false;
	}
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
	
	if (!(isClicked))
	{
		int mx = txMouseX ();
		int my = txMouseY ();
		lastClick = {.x = txMouseX (), .y = txMouseY ()};
		wasClicked = true;
	}
}

void ColorButton::onClick ()
{
	mainCanvas->drawColor = color;
}

void CleanButton::onClick ()
{
	mainCanvas->clearBackground = true;
}