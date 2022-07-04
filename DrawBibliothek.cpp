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

    /*
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
    */
    return false;
}


void compressImage$ (DebugInfo info, AbstractAppData* app, HDC &newDC, Vector newSize, HDC oldDC, Vector oldSize, int line/* = NULL*/)
{
    qassert (oldDC, info);
	char str[10] = {};
	sprintf(str, "%d", line);
		//newSize.print (str);

    //if (!newDC) app->deleteDC (newDC);
	newDC = app->createDIBSection (newSize.x, newSize.y);
    qassert (newDC, info);

	qassert (StretchBlt (newDC, 0, 0, newSize.x, newSize.y, oldDC, 0, 0, oldSize.x, oldSize.y, SRCCOPY), info);

 }







void compressDraw$(DebugInfo info, AbstractAppData* app, HDC finalDC, Vector pos, Vector finalSize, HDC dc, Vector originalSize, int line/* = NULL*/)
{
    qassert (finalDC && dc, info);
	HDC copyOfDC = NULL;
	//if (test1)printBlt (dc);
	compressImage (app, copyOfDC, finalSize, dc, originalSize, line);
	//printBlt (dc);
	app->bitBlt (finalDC, pos.x, pos.y, finalSize.x, finalSize.y, copyOfDC);
	
    app->deleteDC (copyOfDC);
}


/*
void selectFont$ (DebugInfo info, const char *text, int font, HDC dc)
{
    txSelectFont ("Arial", font, -1, FW_DONTCARE, false, false, false, 0, dc);    
}
*/



void standartDraw$ (DebugInfo info, Window *window)
{
    qassert (window, info);
    $s

    AbstractAppData* app = window->app;
    assert(app);

	if (window->finalDC) app->setColor(window->color, window->finalDC);
	if (window->finalDC) app->rectangle (0, 0, window->rect.getSize ().x, window->rect.getSize ().y, window->finalDC);
                                                                                           
    if (window->text)
    {
        app->setColor(window->systemSettings->TextColor, window->finalDC);
        app->selectFont("Arial", window->font, window->finalDC);
        app->drawText(0, 0, window->rect.getSize().x, window->rect.getSize().y, window->text, window->finalDC, window->format);
    }
                                                                                          
	if (window->dc)                                                                                
	{                                                                                           
		//compressDraw (app, window->finalDC, {0, 0}, window->rect.getSize (), window->dc, window->originalRect.getSize ());
        app->bitBlt(window->finalDC, 0, 0 , window->rect.getSize().x, window->rect.getSize().y, window->dc);
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

    bool missClicked = true;

    int returnableVal = -1;

    //if (HideIfIsNotActive) unHide ();

	if (manager->advancedMode)
	{
		manager->clickHandle();
		for (int i = manager->currLen - 1; i >= 0; i--)
		{
			if (manager->pointers[i]->rect.inRect(mp))
			{
				clickButton (manager->pointers[i], manager, mp);

				missClicked = false;
                returnableVal = i;
				if (manager->pointers[i]->advancedMode) break;
			}
			else
			{
				missClicked = true;
			}
		}
	}

    return returnableVal;
}



void standartManagerDraw$ (DebugInfo info, Manager *manager)
{
    qassert (manager, info);

    manager->controlHandle ();

    AbstractAppData* app = manager->app;
    assert(app);
	if (manager->dc) app->bitBlt (manager->finalDC, 0, 0, 0, 0, manager->dc);

    int test1 = 0;
	for (int i = 0; i < manager->currLen; i++)
	{
		if (manager->pointers[i]->advancedMode && manager->pointers[i]->reDraw) manager->pointers[i]->draw ();
 		if (manager->pointers[i]->advancedMode) 
		{
            if (manager->pointers[i]->needTransparencyOutput) app->transparentBlt(manager->finalDC, manager->pointers[i]->rect.pos.x, manager->pointers[i]->rect.pos.y, manager->pointers[i]->rect.getSize().x, manager->pointers[i]->rect.getSize().y, manager->pointers[i]->finalDC);
            else                                              app->bitBlt(manager->finalDC, manager->pointers[i]->rect.pos.x, manager->pointers[i]->rect.pos.y, manager->pointers[i]->rect.getSize().x, manager->pointers[i]->rect.getSize().y, manager->pointers[i]->finalDC);
		}
	}
}






void swap$ (DebugInfo info, int &x0, int &y0)
{
    assert (&x0 && &y0);
    int copy = x0;
    x0 = y0;
    y0 = copy;
}




bool Manager::addWindow (Window *window)
{
    assert (window);
	if (currLen >= length)
    {
        printf ("!!!Unable to add new Window!!!\n");
        return 0;
    }

	pointers[currLen] = window;
    currLen++;

	window->manager = this;
	
	return 1;
}


void Window::print (HDC DC)
{
    assert (DC);
	draw();
	app->bitBlt (DC, rect.pos.x, rect.pos.y, rect.getSize().x, rect.getSize().y, finalDC);
}

Vector Window::getSize()
{
	return this->rect.finishPos - this->rect.pos;
}

void Window::MoveWindowTo(Vector pos)
{
    Vector size = getSize();

    rect.pos = pos;
    rect.finishPos = rect.pos + size;
}

void Window::MoveWindow(Vector delta)
{
    rect.pos += delta;
    rect.finishPos += delta;
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

    if (systemSettings->debugMode == 2) printf("newRect {%lf, %lf}; {%lf, %lf}\n", newRect.pos.x, newRect.pos.y, newRect.finishPos.x, newRect.finishPos.y);
	if (newRect.getSize().x > 0 && newRect.getSize().y > 0)
	{
		finalDCSize = {newRect.getSize().x, newRect.getSize().y};
        if (systemSettings->debugMode == 2) printf("finalDCSize {%lf, %lf}; \n", finalDCSize.x, finalDCSize.y);

        app->deleteDC(finalDC);
		finalDC = app->createDIBSection(finalDCSize, &finalDCArr);

		app->setColor(color, finalDC);
		app->rectangle(0, 0, newRect.getSize().x, newRect.getSize().y, finalDC);
		if (systemSettings->debugMode == 5) app->drawOnScreen(finalDC);
	}
    rect = newRect;
}

void Window::reInit ()
{
    if (rect.getSize().x > 0 && rect.getSize().y > 0)
	{
			finalDCSize = {rect.getSize().x, rect.getSize().y};
			finalDC = app->createDIBSection(finalDCSize.x, finalDCSize.y, &finalDCArr);
            app->setColor(color, finalDC);
            app->rectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);
			if (systemSettings->debugMode == 5) app->drawOnScreen(finalDC);
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
    //POINT mousePos = {(double)txMouseX() - getAbsCoordinats(coordinatsWithHandle).x, (double) txMouseY () - getAbsCoordinats(coordinatsWithHandle).y};
    //ScreenToClient (systemSettings->MAINWINDOW, &mousePos);
    //return {(double)mousePos.x, (double)mousePos.y};
    return { 0, 0 };
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



void Manager::draw ()
{
    standartManagerDraw (this);	
}

bool Manager::clickHandle ()
{
	if (handle.rect.inRect (getMousePos()))
	{
		startCursorPos.x = getAbsMousePos().x;
		startCursorPos.y = getAbsMousePos().y;
        handle.setMbLastTime();
        return true;
	}
    return false;
}

void Manager::controlHandle ()
{
    bool isClickedAgo = handle.isClickedLastTime();
    if (app->systemSettings->debugMode == 5) printf("isClickedLastTime: %d\n", isClickedAgo);

    Vector absMP = getAbsMousePos ();

    if (app->systemSettings->debugMode == 5) printf("absMP: {%lf, %lf}\n", absMP.x, absMP.y);

	if (isClickedAgo && manager)
	{
		rect.pos.x += absMP.x - startCursorPos.x;
		rect.pos.y += absMP.y - startCursorPos.y;
		rect.finishPos.x += absMP.x - startCursorPos.x;
		rect.finishPos.y += absMP.y - startCursorPos.y;
		startCursorPos.x = absMP.x;
		startCursorPos.y = absMP.y;
	}
    if (getMBCondition () == 0) handle.setMbLastTime();
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
	Window* copyOfWindow = pointers[currLen - 1];
    

	for (int i = currLen - 1; i > numOfWindow; i--)
	{
		Window* preCopyOfWindow = pointers[i - 1];
		pointers[i - 1] = copyOfWindow;
		copyOfWindow = preCopyOfWindow;
	}

	if (numOfWindow < currLen - 1)
	{
		pointers[currLen - 1] = copyOfStartWindow;
	}
    

}

void Manager::onClick (Vector mp)
{
    standartManagerOnClick (this, mp);	
}

void clickButton (Window *window, Manager *manager, Vector mp)
{
	window->onClick (mp - window->rect.pos);
}
