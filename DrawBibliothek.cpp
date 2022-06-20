#pragma once
#include "DrawBibliothek.h"




void txSetAllColors$ (DebugInfo info, COLORREF color, HDC dc /*= txDc ()*/, int thickness/* = 1*/)
{
    qassert (dc, info);
	txSetFillColor (color, dc); 
	txSetColor (color, thickness, dc);
}

bool drag$ (DebugInfo info, Vector *objPos, Vector *lastTimePos, bool *dragedLastTime, bool clicked)
{
    qassert (objPos && lastTimePos, info);

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
	if (clicked != 1)
	{
		*dragedLastTime = false;
        return true;//true if finished
	}
    return false;
}


void compressImage$ (DebugInfo info, HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize, int line)
{
    qassert (oldDC, info);
	char str[10] = {};
	sprintf(str, "%d", line);
		//newSize.print (str);

    if (!newDC) txDeleteDC (newDC);
	newDC = txCreateCompatibleDC (newSize.x, newSize.y);
    qassert (newDC, info);

	qassert (StretchBlt (newDC, 0, 0, newSize.x, newSize.y, oldDC, 0, 0, oldSize.x, oldSize.y, SRCCOPY), info);

 }







void compressDraw$ (DebugInfo info, HDC finalDC, Vector pos, Vector finalSize, HDC dc, Vector originalSize, int line)
{
    qassert (finalDC && dc, info);
	HDC copyOfDC = NULL;
	//if (test1)printBlt (dc);
	compressImage (copyOfDC, finalSize, dc, originalSize, line);
	//printBlt (dc);
	txBitBlt (finalDC, pos.x, pos.y, finalSize.x, finalSize.y, copyOfDC);
	
    txDeleteDC (copyOfDC);
}


void selectFont$ (DebugInfo info, const char *text, int font, HDC dc)
{
    txSelectFont ("Arial", font, -1, FW_DONTCARE, false, false, false, 0, dc);    
}



void standartDraw$ (DebugInfo info, Window *window)
{
    qassert (window, info);
    $s                                                                                     
	if (window->finalDC) txSetAllColors (window->color, window->finalDC);                                           
	if (window->finalDC) txRectangle (0, 0, window->rect.getSize ().x, window->rect.getSize ().y, window->finalDC);        
                                                                                           
                                                                                          
	if (window->finalDC) txSetAllColors (window->systemSettings->TextColor, window->finalDC);                                     
	txSetTextAlign (TA_CENTER, window->finalDC);
    selectFont ("Arial", window->font, window->finalDC);
    txDrawText (window->sideThickness, window->sideThickness, window->rect.getSize().x, window->rect.getSize().y, window->text, window->format, window->finalDC);
                                                                                           
                                                                                          
	if (window->dc)                                                                                
	{                                                                                           
		compressDraw (window->finalDC, {0, 0}, window->rect.getSize (), window->dc, window->originalRect.getSize ());
	}                                                                                         
}


const char* getCustomFilePath(const char* question)
{
    char fileName[MAX_PATH] = "";

    OPENFILENAMEW ofn = { sizeof(ofn), txWindow() }; 

    ofn.lpstrTitle = (LPCWSTR)question;

    ofn.lpstrFile = (LPWSTR)fileName;
    ofn.nMaxFile = sizeof(fileName);

    ofn.lpstrInitialDir = NULL;

    ofn.Flags = OFN_PATHMUSTEXIST;

    if ((GetOpenFileNameW))
        (GetOpenFileNameW(&ofn));


    return fileName;
}


const char* getCustomFilePathForSaving(const char* question, const char* fileTypeDescribtion, const char* fileType)
{
    char fileName[MAX_PATH] = "";

    OPENFILENAME ofn = { sizeof(OPENFILENAME), txWindow() };

    ofn.lpstrTitle = question;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = sizeof(fileName);
    ofn.lpstrFilter = fileTypeDescribtion;
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = fileType;


    bool oldPSW = _txProcessSystemWarnings;
    _txProcessSystemWarnings = false;//отключает всякие системные проверки тхлибом иначе возникает ошибка 298

    if ((GetSaveFileNameA))
        (GetSaveFileNameA(&ofn));

    _txProcessSystemWarnings = oldPSW;

    return fileName;
}





int standartManagerOnClick$ (DebugInfo info, Manager *manager, Vector mp)
{
    qassert (manager, info);

    manager->mousePos = mp;

    bool missClicked = true;

    int returnableVal = -1;

    //if (HideIfIsNotActive) unHide ();

	if (manager->advancedMode)
	{
		manager->clickHandle();
		for (int i = manager->newButtonNum - 1; i >= 0; i--)
		{
			if (manager->pointers[i]->rect.inRect(mp))
			{
				clickButton (manager->pointers[i], manager, mp);

				missClicked = false;
                returnableVal=  i;
				if (manager->pointers[i]->advancedMode) break;
			}
			else
			{
				manager->pointers[i]->isClicked = false;

				missClicked = true;
			}
		}
	}

	if (missClicked == true) manager->activeWindow = NULL;

    return returnableVal;
}



void standartManagerDraw$ (DebugInfo info, Manager *manager)
{
    qassert (manager, info);

    manager->controlHandle ();

    //if (manager->HideIfIsNotActive && manager->activeWindow != this) 
    //{
        //hide ();
    //}

	txSetAllColors (manager->color, manager->finalDC);
	//txRectangle (0, 0, DCMAXSIZE, DCMAXSIZE, manager->finalDC);
	if (manager->dc) txBitBlt (manager->finalDC, 0, 0, 0, 0, manager->dc);

    manager->controlMouse ();


	for (int i = 0; i < manager->newButtonNum; i++)
	{
        

		if (manager->pointers[i]->advancedMode && manager->pointers[i]->reDraw) manager->pointers[i]->draw ();
 		if (manager->pointers[i]->advancedMode) 
		{
			txBitBlt (manager->finalDC, manager->pointers[i]->rect.pos.x, manager->pointers[i]->rect.pos.y, manager->pointers[i]->rect.getSize().x, manager->pointers[i]->rect.getSize().y, manager->pointers[i]->finalDC);
			//bitBlt (finalDCArr, pointers[i]->rect.pos.x, pointers[i]->rect.pos.y, pointers[i]->rect.getSize().x, pointers[i]->rect.getSize().y, pointers[i]->finalDCArr, pointers[i]->finalDCSize.x, pointers[i]->finalDCSize.y, finalDCSize.x, finalDCSize.y);	
			//printBlt (pointers[i]->finalDC);
		}
		if (manager->clicked != 1)
		{
			manager->pointers[i]->isClicked = false;
		}
	}

	if (manager->getActiveWindow () != manager && manager->manager) 
	{
		manager->activeWindow = NULL;
    }
}






void swap$ (DebugInfo info, int &x0, int &y0)
{
    assert (&x0 && &y0);
    int copy = x0;
    x0 = y0;
    y0 = copy;
}




Window* Manager::getActiveWindow ()
{
	if (this == NULL) return 0;

	return activeWindow;
}


bool Manager::addWindow (Window *window)
{
    assert (window);
	if (newButtonNum >= length)
    {
        //printf ("!!!Unable to add new Window!!!\n");
        return 0;
    }

	pointers[newButtonNum] = window;
	newButtonNum++;

	window->manager = this;
	
	return 1;
}


void Window::print (HDC DC)
{
    assert (DC);
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
    //assert (newRect.isValid());

    if (systemSettings->debugMode) printf("newRect {%lf, %lf}; {%lf, %lf}\n", newRect.pos.x, newRect.pos.y, newRect.finishPos.x, newRect.finishPos.y);
	if (newRect.getSize().x > 0 && newRect.getSize().y > 0)
	{
		finalDCSize = {newRect.getSize().x, newRect.getSize().y};
        if (systemSettings->debugMode) printf("finalDCSize {%lf, %lf}; \n", finalDCSize.x, finalDCSize.y);
		finalDC = txCreateDIBSection(finalDCSize.x, finalDCSize.y, &finalDCArr);
		txSetAllColors(color, finalDC);
		txRectangle(0, 0, newRect.getSize().x, newRect.getSize().y, finalDC);
		if (systemSettings->debugMode == 5) printBlt(finalDC);
	}
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
			if (systemSettings->debugMode == 5) printBlt(finalDC);
	}

	originalRect = rect;
}

/*
void Window::resize (Vector newSize, Vector oldSize)
{
	assert (newSize > 0 && oldSize > 0);
	if (!dc) return;
	//compressImage (dc, newSize, {rect.getSize().x, rect.getSize().y});
	//if (test1) printBlt (dc);
	rect = {.pos = rect.pos * newSize / oldSize, .finishPos = rect.finishPos * newSize / oldSize};
}     */

void Window::setStartRect (Vector pos, Vector finishPos)
{
    assert (pos > 0 && finishPos > 0);
	rect = {pos, finishPos};
	originalRect = rect;
}
Vector Window::getRelativeMousePos (bool coordinatsWithHandle)
{
    POINT mousePos = {(double)txMouseX() - getAbsCoordinats(coordinatsWithHandle).x, (double) txMouseY () - getAbsCoordinats(coordinatsWithHandle).y};
    ScreenToClient (systemSettings->MAINWINDOW, &mousePos);
    return {(double)mousePos.x, (double)mousePos.y};
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

/*
Vector windowMousePos(bool isThisMainFile /* = true)
{
    POINT mousePos = {(double)txMouseX(), (double)txMouseY()};

    if (!isThisMainFile)
    {
        ScreenToClient (systemSettings->MAINWINDOW, &mousePos);
    }
    return {(double)mousePos.x, (double)mousePos.y};
}
*/

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



Vector Manager::getMousePos()
{
    return mousePos;
}

void Manager::draw ()
{
    standartManagerDraw (this);	
}

bool Manager::clickHandle ()
{
	if (handle.rect.inRect (mousePos))
	{
		startCursorPos.x = manager->getMousePos().x;
		startCursorPos.y = manager->getMousePos().y;
		handle.isClicked = true;
        return true;
	}
    return false;
}

void Manager::controlHandle ()
{
    
	if (handle.isClicked)
	{
		rect.pos.x += manager->getMousePos().x - startCursorPos.x;
		rect.pos.y += manager->getMousePos().y - startCursorPos.y;
		rect.finishPos.x += manager->getMousePos().x - startCursorPos.x;
		rect.finishPos.y += manager->getMousePos().y - startCursorPos.y;
		startCursorPos.x = manager->getMousePos().x;
		startCursorPos.y = manager->getMousePos().y;
        //printf ("mouse == 1\n");
	}
	if (clicked != 1)
	{
		handle.isClicked = false;
        //printf ("mouse == 0\n");
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

void Manager::controlMouse ()
{
    for (int i = 0; i < newButtonNum; i++)
    {
        assert (pointers[i]);
        pointers[i]->mousePos = mousePos - pointers[i]->rect.pos;
    }

    if (clicked >= 1) return;
    for (int i = 0; i < newButtonNum; i++)
    {
        pointers[i]->clicked = 0;
        pointers[i]->isClicked = 0;
    }
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

void Manager::onClick (Vector mp)
{
    standartManagerOnClick (this, mp);	
}

void clickButton (Window *window, Manager *manager, Vector mp)
{
    manager->activeWindow = window;
    window->clicked = manager->clicked;
	window->onClick (mp - window->rect.pos);
	window->isClicked = true;
}
