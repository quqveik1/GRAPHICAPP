
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "TXLib.h"
#include "Q_Rect.h"
#include "Macroses.h"
#include "GlobalOptions.h"
#include "DebugInfo.h"
#include "LoadManager.h"
#include "Tool.h"
#include "ProgrammeDate.h"
#include "commdlg.h"



struct Manager;
struct Window;



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
const char* getCustomFilePath(const char* question);
const char* getCustomFilePathForSaving(const char* question, const char* fileTypeDescribtion = NULL, const char* fileType = NULL);







struct Window
{
    CSystemSettings* systemSettings = NULL;

    Rect rect = {};
	Rect originalRect;
	COLORREF color;

	const char *text;
    int format;
    int font;
    int sideThickness;

	bool isClicked;
	HDC dc;

	HDC finalDC = NULL;
	RGBQUAD *finalDCArr = NULL;
    Vector finalDCSize = {}; 
	Manager *manager;
	bool advancedMode;
	bool reDraw;

    CLoadManager* loadManager;

    Vector mousePos = {};
    int clicked = false;

    Window (CSystemSettings* _systemSettings, Rect _rect = {}, COLORREF _color = NULL, HDC _dc = NULL, Manager *_manager = NULL, const char *_text = "", bool _advancedMode = true, CLoadManager* _loadManager = NULL) :
        systemSettings (_systemSettings),
		rect (_rect),
		color(_color),
		manager (_manager),
		text (_text), 
		isClicked (false), 
		dc (_dc),
		advancedMode (_advancedMode),
		reDraw (true),
        loadManager (_loadManager), 
        font (_systemSettings->MainFont),
        sideThickness (_systemSettings->SIDETHICKNESS),
        format (_systemSettings->TEXTFORMAT)

	{
        if (systemSettings->debugMode) printf("rect {%lf, %lf}; {%lf, %lf}\n", rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

        if (!color) color = systemSettings->MenuColor;
		resize (rect);

        if (!color) color = systemSettings->MenuColor;

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

	Manager (CSystemSettings* _systemSettings,  Rect _rect,  int _length, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}, COLORREF _color = NULL, bool _coordinatSysFromHandle = false, bool _HideIfIsNotActive = false, CLoadManager* _loadManager = NULL) :
		Window (_systemSettings, _rect, _color, _dc, NULL, "", _advancedMode, _loadManager),
        handle (_systemSettings, _handle),
		length (_length),
		pointers (new Window* [_length]{}),
		newButtonNum (0),
		activeWindow (NULL),
		startCursorPos({}),
		coordinatSysFromHandle (_coordinatSysFromHandle),
        HideIfIsNotActive (_HideIfIsNotActive) 
	{
		handle.manager = this;
		handle.rect.finishPos.x = systemSettings->DCMAXSIZE * 10;
		handle.color = systemSettings->MenuColor;
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




