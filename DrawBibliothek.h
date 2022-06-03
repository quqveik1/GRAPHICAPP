
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "TXLib.h"
#include "..\Q_Rect.h"
#include "..\Macroses.h"
#include "GlobalOptions.h"
#include "DebugInfo.h"
#include "LoadManager.h"
#include "Tool.h"
#include "ProgrammeDate.h"

CLoadManager LoadManager;

struct Manager;
struct Window;

Lay *ActiveLay = NULL;



struct CHistoryStep
{
	int toolsNum = 0;
	Tool* tools;
    void* toolsData = NULL;
	int thickness = 1;
};




void txSetAllColors$(DebugInfo info, COLORREF color, HDC dc = txDC(), int thickness = 1);
#define txSetAllColors(...) txSetAllColors$ (getDebugInfo, __VA_ARGS__)
void compressDraw$ (DebugInfo info, HDC finalDC, Vector pos, Vector finalSize, HDC dc, Vector originalSize, int line = NULL);
#define compressDraw(...) compressDraw$ (getDebugInfo, __VA_ARGS__)
void compressImage$ (DebugInfo info, HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize, int line = NULL);
#define compressImage(...) compressImage$ (getDebugInfo, __VA_ARGS__)
bool drag$ (DebugInfo info, Vector *objPos, Vector *lastTimePos, bool *dragedLastTime, bool clicked);
#define drag(...) drag$ (getDebugInfo, __VA_ARGS__)
int standartManagerOnClick$ (DebugInfo info, Manager *manager, Vector mp);
#define standartManagerOnClick(...) standartManagerOnClick$ (getDebugInfo, __VA_ARGS__);;
void standartDraw$ (DebugInfo info, Window *window);
#define standartDraw(...) standartDraw$ (getDebugInfo, __VA_ARGS__)
void standartManagerDraw$ (DebugInfo info, Manager *manager);
#define standartManagerDraw(...) standartManagerDraw$ (getDebugInfo, __VA_ARGS__)
void clickButton (Window *window, Manager *manager, Vector mp);
void selectFont$ (DebugInfo info, const char *text, int font, HDC dc);
#define selectFont(...) selectFont$ (getDebugInfo, __VA_ARGS__)
void swap$ (DebugInfo info, int &x0, int &y0);
#define swap(...) swap$ (getDebugInfo, __VA_ARGS__)
Vector windowMousePos(bool isThisMainFile = true);







struct Window
{
	Rect rect;
	Rect originalRect;
	COLORREF color;

	const char *text;
    int format = DT_CENTER;
    int font = MainFont;
    int sideThickness = SIDETHICKNESS;

	bool isClicked;
	HDC dc;

	HDC finalDC = NULL;
	RGBQUAD *finalDCArr = NULL;
    Vector finalDCSize = {}; 
	Manager *manager;
	bool advancedMode;
	bool reDraw;

    Vector mousePos = {};
    int clicked = false;

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
		resize (rect);

		originalRect = rect;
	} 

    ~Window ()
    {
        if (dc) txDeleteDC (dc);
        if (finalDC) txDeleteDC (dc);
    }


	Vector getAbsCoordinats (bool coordinatsWithHandle = false);
    Vector getRelativeMousePos (bool coordinatsWithHandle = false);
	Rect getAbsRect (bool coordinatsWithHandle = false);
	
	virtual void resize (Rect newSize);
	//void resize (Vector newSize, Vector oldSize);
    void reInit ();
	void setStartRect (Vector pos, Vector finishPos);
	void print (HDC finalDC);
    

	Vector getSize();
	virtual void draw ();
	virtual void onClick (Vector mp) {};

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
    bool HideIfIsNotActive;

	Manager (Rect _rect,  int _length, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}, COLORREF _color = MenuColor, bool _coordinatSysFromHandle = false, bool _HideIfIsNotActive = false) :
		Window (_rect, _color, _dc, NULL, "", _advancedMode),
		length (_length),
		pointers (new Window*[_length]{}),
		newButtonNum (0),
		activeWindow (NULL), 
		handle (_handle),
		startCursorPos({}),
		coordinatSysFromHandle (_coordinatSysFromHandle),
        HideIfIsNotActive (_HideIfIsNotActive) 
	{
		handle.manager = this;
		handle.rect.finishPos.x = DCMAXSIZE * 10;
		handle.color = MenuColor;
	}

    ~Manager()
    {
        if (dc) txDeleteDC(dc);
        if (finalDC) txDeleteDC(finalDC);
        for (int i = 0; i < length; i++)
        {
            if (pointers[i]) delete pointers[i];
        }
    }

	bool addWindow (Window *window);
	Window *getActiveWindow ();
	void controlHandle ();
	bool clickHandle ();
	void replaceWindow (int numOfWindow);
    void hide ();
    void unHide ();

    void controlMouse ();

	virtual void draw () override;
	virtual void onClick (Vector mp) override;

    Vector getMousePos();
};




