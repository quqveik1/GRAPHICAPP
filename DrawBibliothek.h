
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "TXLib.cpp"
#include "Q_Rect.h"
#include "Macroses.h"
#include "GlobalOptions.h"
#include "DebugInfo.h"
#include "LoadManager.h"
#include "Tool.h"
#include "ProgrammeDate.h"
#include "commdlg.h"
#include "TransferStructure.h"




struct Manager;
struct Window;



struct CHistoryStep
{
	int toolsNum = 0;
	Tool* tools = NULL;
    void* toolsData = NULL;
	int thickness = 1;
};




void txSetAllColors$(DebugInfo info, COLORREF color, HDC dc = txDC(), int thickness = 1);
#define txSetAllColors(...) txSetAllColors$ (getDebugInfo, __VA_ARGS__)
void compressDraw$ (DebugInfo info, AbstractAppData* app, HDC finalDC, Vector pos, Vector finalSize, HDC dc, Vector originalSize, int line = NULL);
#define compressDraw(...) compressDraw$ (getDebugInfo, __VA_ARGS__)
void compressImage$(DebugInfo info, AbstractAppData* app, HDC& newDC, Vector newSize, HDC oldDC, Vector oldSize, int line = NULL);
#define compressImage(...) compressImage$ (getDebugInfo, __VA_ARGS__)
bool drag$ (DebugInfo info, Vector *objPos, Vector *lastTimePos, bool *dragedLastTime, bool getMBCondition());
#define drag(...) drag$ (getDebugInfo, __VA_ARGS__)
int standartManagerOnClick$ (DebugInfo info, Manager *manager, Vector mp);
#define standartManagerOnClick(...) standartManagerOnClick$ (getDebugInfo, __VA_ARGS__);;
void standartDraw$ (DebugInfo info, Window *window);
#define standartDraw(...) standartDraw$ (getDebugInfo, __VA_ARGS__)
void standartManagerDraw$ (DebugInfo info, Manager *manager);
#define standartManagerDraw(...) standartManagerDraw$ (getDebugInfo, __VA_ARGS__)
void clickButton (Window *window, Manager *manager, Vector mp);
void selectFont$ (DebugInfo info, const char *text, int font, HDC dc);
//#define selectFont(...) selectFont$ (getDebugInfo, __VA_ARGS__)
void swap$ (DebugInfo info, int &x0, int &y0);
#define swap(...) swap$ (getDebugInfo, __VA_ARGS__)
Vector windowMousePos(bool isThisMainFile = true);
const char* getCustomFilePath(const char* question);
const char* getCustomFilePathForSaving(const char* question, const char* fileTypeDescribtion = NULL, const char* fileType = NULL);







struct Window
{
    CSystemSettings* systemSettings = NULL;
    AbstractAppData* app = NULL;

    Rect rect = {};
	Rect originalRect;
	COLORREF color;

	const char *text;
    int format;
    int font;
    int sideThickness;

    bool redrawStatus = false;

	HDC dc;

	HDC finalDC = NULL;
	RGBQUAD *finalDCArr = NULL;
    Vector finalDCSize = {}; 
	Manager *manager;
	bool advancedMode;
	bool reDraw;

    CLoadManager* loadManager;

    Vector mousePos = {};
    int clicked = 0;
    int mbLastTime = 0;

    Window (AbstractAppData* _app, Rect _rect = {}, COLORREF _color = NULL, HDC _dc = NULL, Manager *_manager = NULL, const char *_text = "", bool _advancedMode = true) :
        app (_app),
        systemSettings (_app->systemSettings),
		rect (_rect),
		color(_color),
		manager (_manager),
		text (_text), 
		dc (_dc),
		advancedMode (_advancedMode),
		reDraw (true),
        loadManager (_app->loadManager),
        font (_app->systemSettings->MainFont),
        sideThickness (_app->systemSettings->SIDETHICKNESS),
        format (_app->systemSettings->TEXTFORMAT)

	{
        if (systemSettings->debugMode >= 0) printf("rect {%lf, %lf}; {%lf, %lf}\n", rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

        if (!color) color = systemSettings->MenuColor;
		resize (rect);

        if (!color) color = systemSettings->MenuColor;

		originalRect = rect;
	} 

    ~Window ()
    {
        assert(app);
        if (dc) app->deleteDC(dc);
        if (finalDC) app->deleteDC(dc);
    }


	Vector getAbsCoordinats (bool coordinatsWithHandle = false);
    Vector getRelativeMousePos (bool coordinatsWithHandle = false);
	Rect getAbsRect (bool coordinatsWithHandle = false);

    
	
	virtual void resize (Rect newSize);
	//void resize (Vector newSize, Vector oldSize);
    virtual void reInit ();
    virtual void setStartRect (Vector pos, Vector finishPos);
    virtual void print (HDC finalDC);
    

    virtual Vector getSize();
    virtual Manager* getManager() { return manager; };
    virtual void needRedraw() {};
    virtual bool getRadrawStatus() { return redrawStatus; };
    virtual void noMoreRedraw() { redrawStatus = false; };

    virtual void MoveWindow(Vector pos);
    virtual void MoveWindowTo(Vector delta);


    virtual int getMBCondition() {
        if (getManager()) return ((Window*)getManager())->getMBCondition();
        else              return clicked; };
    virtual bool isClickedLastTime() { 
        if (mbLastTime == 0) return false;
        else                 return mbLastTime == getMBCondition(); };

    virtual void setMbLastTime() { mbLastTime = getMBCondition(); };

    virtual Vector getMousePos() {
        if (getManager()) return ((Window*)getManager())->getMousePos() - rect.pos;
        else              return mousePos; };

    virtual Vector getAbsMousePos() { return getMousePos() + rect.pos; };


	virtual void draw ();
	virtual void onClick (Vector mp) {};

	virtual void deleteButton ();
};

struct Manager : Window
{
	int length;
	Window **pointers = NULL;
	int currLen;
	Window *activeWindow;
	Window handle;
	Vector startCursorPos;
	bool coordinatSysFromHandle;
    bool HideIfIsNotActive;

	Manager (AbstractAppData* _app, Rect _rect,  int _length, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}, COLORREF _color = NULL, bool _coordinatSysFromHandle = false, bool _HideIfIsNotActive = false) :
		Window (_app, _rect, _color, _dc, NULL, "", _advancedMode),
        handle (_app, _handle),
		length (_length),
		pointers (new Window* [_length]{}),
		currLen (0),
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
        assert(app);
        if (dc) app->deleteDC(dc);
        if (finalDC) app->deleteDC(finalDC);
        for (int i = 0; i < length; i++)
        {
            if (pointers[i]) delete pointers[i];
        }
    }

    virtual bool addWindow (Window *window);
    virtual Window *getActiveWindow ();
    virtual void controlHandle ();
    virtual bool clickHandle ();
    virtual void replaceWindow (int numOfWindow);
    virtual void hide ();
    virtual void unHide ();
    virtual int& getCurLen() { return currLen; };

    virtual void redraw() { redrawStatus = true; };

	virtual void draw ()             override;
	virtual void onClick (Vector mp) override;

};




