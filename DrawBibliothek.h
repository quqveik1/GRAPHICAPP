#pragma once
#include "TXLib.h"
#include "..\Q_Rect.h"
#include "..\Macroses.h"
#include "..\GlobalOptions.h"

struct Manager;
struct Windows;


void txSetAllColors(COLORREF color, HDC dc = txDC(), int thickness = 1);
void compressDraw (HDC finalDC, Vector pos, Vector finalSize, HDC dc, Vector originalSize, int line = NULL);
void compressImage (HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize, int line = NULL);
bool drag(Vector *objPos, Vector *lastTimePos, bool *dragedLastTime);


bool drag(Vector *objPos, Vector *lastTimePos, bool *dragedLastTime)
{
    int mx = txMouseX();
    int my = txMouseY();
    if (*dragedLastTime == false && txMouseButtons() == 1)
    {
        lastTimePos->x = mx;
        lastTimePos->y = my;
        *dragedLastTime = true;
        //printf ("START");
    }
    if (*dragedLastTime)
	{
		objPos->x += mx - lastTimePos->x;
		objPos->y += my - lastTimePos->y;
        //printf ("[objpos: {%lf, %lf} | lastTimePos: {%lf, %lf} | delta: {%lf, %lf}]\n", objPos->x, objPos->y, lastTimePos->x, lastTimePos->y, mx - lastTimePos->x,  my - lastTimePos->y);
		lastTimePos->x = mx; 
		lastTimePos->y = my; 

        
	}
	if (txMouseButtons () != 1)
	{
		*dragedLastTime = false;
        return true;//true if finished
	}
    return false;
}



void txSetAllColors (COLORREF color, HDC dc /*= txDc ()*/, int thickness)
{
	txSetFillColor (color, dc); 
	txSetColor (color, thickness, dc);
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


void compressImage (HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize, int line)
{

	char str[10] = {};
	sprintf(str, "%d", line);
	//if (newSize.x <= 0 || newSize.y <= 0)
		newSize.print (str);

	newDC = txCreateCompatibleDC (newSize.x, newSize.y);

	
	//if (test1)printBlt (oldDC);

	//assert (oldDC);
	if (!StretchBlt (newDC, 0, 0, newSize.x, newSize.y, oldDC, 0, 0, oldSize.x, oldSize.y, SRCCOPY))
	{
		//printBlt (newDC);
		printBlt (oldDC);
	}

	if (test1)printBlt (newDC);
}

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
    void reInit ();
	void setStartRect (Vector pos, Vector finishPos);
	void print (HDC finalDC);
    

	Vector getSize();
	virtual void draw ();
	virtual void onClick () {};

	virtual void deleteButton ();
};

void standartDraw (Window *window);

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
		pointers (new Window*[length]{}),
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

	bool addWindow (Window *window);
	Window *getActiveWindow ();
	void controlHandle ();
	void clickHandle ();
	void replaceWindow (int numOfWindow);
	void controlSize ();
    void hide ();
    void unHide ();

	virtual void draw () override;
	virtual void onClick () override;

	virtual void deleteButton () override;
};


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
	standartDraw(this);
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

void Window::reInit ()
{
    if (rect.getSize().x > 0 && rect.getSize().y > 0)
	{
			finalDCSize = {rect.getSize().x, rect.getSize().y};
			finalDC = txCreateDIBSection(finalDCSize.x, finalDCSize.y, &finalDCArr);
			txSetAllColors(color, finalDC);
			txRectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);
			if (test1) printBlt(finalDC);
	}

	originalRect = rect;
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

    if (HideIfIsNotActive && manager->activeWindow != this) 
    {
        //hide ();
    }

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


void Manager::hide ()
{
    advancedMode = false;
}
void Manager::unHide ()
{
    advancedMode = true;
}

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

    //if (HideIfIsNotActive) unHide ();

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



void standartDraw (Window *window)
{
                                                                                         
    $s                                                                                     
	if (window->finalDC) txSetAllColors (window->color, window->finalDC);                                           
	if (window->finalDC) txRectangle (0, 0, window->rect.getSize ().x, window->rect.getSize ().y, window->finalDC);        
                                                                                           
                                                                                          
	if (window->finalDC) txSetAllColors (TextColor, window->finalDC);                                     
	txSetTextAlign (TA_CENTER, window->finalDC);                                                    
                                                                                           
                                                                                          
	if (window->dc)                                                                                
	{                                                                                           
		compressDraw (window->finalDC, {0, 0}, window->rect.getSize (), window->dc, window->originalRect.getSize (), __LINE__);
	}                                                                                         
     
}