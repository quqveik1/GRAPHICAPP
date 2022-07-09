#pragma once
#include "Canvas.h"
#include "ZoneSizeControl.cpp"


void CToolManager::addTool(Tool* tool)
{
    assert(tool);
    if (currentLength < ToolsLength - 1)
    {
        tools[currentLength] = tool;
        currentLength++;
    }

}



void Canvas::createLay()
{
    assert(!(currentLayersLength >= LayersNum));
    lay[currentLayersLength].createLay(app);
    if (currentLayersLength <= LayersNum) currentLayersLength++;

    activeLayNum = currentLayersLength - 1;
}

HDC Canvas::playHistoryDC(int stepBack)
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
    HDC historyDC = app->createDIBSection(canvasSize.x, canvasSize.y);

    if (stepBack <= currentHistoryLength && stepBack < HistoryLength)
    {
        app->bitBlt(historyDC, 0, 0, 0, 0, lastSavedDC);
        //app->drawOnScreen(lastSavedDC);

        for (int i = currentHistoryLength - stepBack - 1; i >= 0; i--)
        {
            int pos = 0;
            //if (currentHistoryLength == HistoryLength - 1)  pos = currentHistoryNumber - 1 + 1 + i;
            if (currentHistoryLength < HistoryLength - 1) pos = currentHistoryNumber - 1 - 1 - i;
            if (pos >= HistoryLength) pos -= HistoryLength;
            // history[pos].tools->load (history[pos].toolsData, historyDC);
        }
    }


    timesShowedHistoryInRow++;
    return	historyDC;
}

void Canvas::deleteButton()
{
    if (dc) app->deleteDC(dc);
    if (finalDC) app->deleteDC(finalDC);
    if (canvas) app->deleteDC(canvas);
    closeCanvas.deleteButton();
    //scrollBarVert.deleteButton();
    //scrollBarHor.deleteButton();
    deleteHistory();

}

void Canvas::startTool()
{
    initToolLay();
}

void Canvas::changeTool()
{
    delete (getActiveLay()->getActiveToolLay()->getToolsData());
    getActiveLay()->needRedraw();

    setToolToToolLay(getActiveLay()->getActiveToolLay());
}

void Canvas::initToolLay()
{

    currentDate->size = { lineThickness, lineThickness };
    addToolLay();
    getActiveLay()->addToolLay(&toolLays[currentToolLength]);

    currentToolLength++;
}

void Canvas::addToolLay()
{
    assert(LayersNum >= currentToolLength);
    activeTool = true;
    Tool* activeTool = getActiveTool();
    ToolLay* toollay = getNewToolLay();
    setToolToToolLay(toollay);
}


void Canvas::setToolToToolLay(ToolLay* toollay)
{
    toollay->tool = getActiveTool();
    if (toollay->tool == NULL) return;
    toollay->toolsData = new char[getActiveTool()->ToolSaveLen]{};
}

void Canvas::setCurrentData()
{
    currentDate->mousePos = getMousePos();
    currentDate->managerPos = getAbsCoordinats();
    currentDate->color = systemSettings->DrawColor;
    currentDate->canvasCoordinats = canvasCoordinats;
    currentDate->backGroundColor = TX_BLACK;
    if (getMBCondition() == 0) currentDate->clickedMB = 0;
}

ToolLay* Canvas::getNewToolLay()
{
    return &(toolLays[currentToolLength]);
}

bool Canvas::isDrawingModeChanged()
{
    return DrawingModeLastTime != systemSettings->DrawingMode;
}

Tool* Canvas::getActiveTool()
{
    if (app->systemSettings->DrawingMode <= 0 || app->toolManager->currentLength <= 0) return NULL;
    return app->toolManager->tools[app->systemSettings->DrawingMode - 1];
}

void Canvas::setActiveToolLayNum(int num)
{
    if (!getActiveLay() || !getActiveLay()->getActiveToolLay()) return;
    getActiveLay()->setActiveToolLayNum(num);
    if (getActiveLay()->getActiveToolLay()->isFinished()) editingMode = 1;
    else editingMode = 0;
};

int Canvas::getLastNotStartedToolNum()
{
    int length = getCurrentToolLengthOnActiveLay();
    return length;
}

void Canvas::onClick(Vector mp)
{
    if (getMBCondition() == 1)
    {
        lastClick = mp;
        if (zoneSizeControl.clickFrame()) return;


        int mx = mp.x;
        int my = mp.y;

        if (getActiveWindow() == this)
        {
            if (closeCanvas.rect.inRect(mx, my))
            {
                needToShow = false;
                return;
            }
        }

        if (clickHandle()) return;

        if (!(isClickedLastTime()) && getActiveWindow() == this)
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
    }


    //independet scenery block++++++++++++++++++++++++++++++++++++++++++++++++++++
    currentDate->clickedMB = getMBCondition();
    //independet scenery block----------------------------------------------------


    setMbLastTime();
}


void Canvas::draw()
{
    app->setColor(TX_BLACK, finalDC);
    app->rectangle(0, 0, getSize().x, getSize().y, finalDC);
    app->setColor(systemSettings->BackgroundColor, finalDC);

    if (systemSettings->debugMode >= 3)  printf("Canvas clicked: %d\n", getMBCondition());


    cleanOutputLay();
    controlLay();

    drawLay();

    CLay* activeLay = getActiveLay();
    if (activeLay) controlTool();

    controlEditLay();

    if (app->getAsyncKeyState('Q'))
    {
        endtillkey('Q');
        //playHistory ();
        createLay();

    }

    if (app->getAsyncKeyState('2'))
    {
        systemSettings->DrawingMode = 2;
    }


    if (getActiveWindow() != this) wasClicked = false;

    

    controlHandle();
    drawOnFinalDC(handle);

    controlSize();
    drawCadre();

    


    drawOnFinalDC(closeCanvas);


    DrawingModeLastTime = systemSettings->DrawingMode;


}



HDC Canvas::getImageForSaving()
{
    HDC notClearedDC = getActiveLay()->lay.lay;

    HDC clearedDC = app->createDIBSection(getActiveLay()->lay.laySize.x, getActiveLay()->lay.laySize.y);

    app->transparentBlt(clearedDC, 0, 0, 0, 0, notClearedDC);

    return clearedDC;
    //�������� HDC ������� ������� ����� �������������
}


int Canvas::getActiveLayNum()
{
    return activeLayNum;
}

int Canvas::getCurrentLayLength()
{
    return  currentLayersLength;
}


CLay* Canvas::getActiveLay()
{
    if (activeLayNum < 0 || !lay[activeLayNum].toolLays) return NULL;
    return &(lay[activeLayNum]);
}


Vector Canvas::getLaySize()
{
    if (getActiveLay())
    {
        return getActiveLay()->lay.laySize;
    }
    return {};
}

int Canvas::getEditingMode()
{
    return editingMode;
}


int Canvas::getCurrentToolLengthOnActiveLay()
{
    if (!getActiveLay()) return 0;
    if (!getActiveLay()->getActiveToolLay()) return 0;
    int length = getActiveLay()->getCurrentSize() - 1;
    return length;
}


void Canvas::controlTool()
{
    if (!activeTool)
    {
        startTool();
    }

    CLay* clay = getActiveLay();
    assert(clay);
    ToolLay* toollay = clay->getActiveToolLay();
    assert(toollay);
    Tool* tool = toollay->getTool();
    bool isFinished = toollay->isFinished();



    setCurrentData();
    if (!isFinished)
    {
        if (isDrawingModeChanged()) changeTool();
        if (systemSettings->debugMode == 5) printf("Num:%d_IsFinished:%d", clay->getActiveToolLayNum(), isFinished);
        if (toollay->useTool(currentDate))
        {
            finishTool();
        }
    }
}

void Canvas::finishTool()
{
    activeTool = false;
}

void Canvas::returnHistory(int stepsBack)
{
    if (!(stepsBack <= currentHistoryLength)) return;
    //HDC historyDC = playHistoryDC();
    //printfDCS ();
    //copyAndDelete(canvas, playHistoryDC(stepsBack));
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

void Canvas::controlFilter()
{
    //if (app->getAsyncKeyState(VK_LEFT)) SecondFilterValue++;
    //if (app->getAsyncKeyState(VK_RIGHT)) SecondFilterValue--;
    //if (app->getAsyncKeyState(VK_DOWN)) FirstFilterValue+=10;
    //if (app->getAsyncKeyState(VK_UP)) FirstFilterValue-=10;
    /*
    filter->algorithm = FilterAlgorithm;


    if (manager->activeWindow != this) return;
    if (confirmFilter)
    {
        confirmFilter = false;
        nonConfirmFilter = false;
        app->bitBlt(canvas, 0, 0, 0, 0, tempFilterDC);
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
    zoneSizeControl.controlFrame();
    closeCanvas.rect.pos = { rect.getSize().x - systemSettings->MENUBUTTONSWIDTH,  0 };
    closeCanvas.rect.finishPos = { rect.getSize().x, systemSettings->HANDLEHEIGHT };
}

void Canvas::controlSizeSliders()
{
    //scrollBarVert.resize ({.pos = {rect.finishPos}});
}






void Canvas::saveHistory()
{
    //app->bitBlt (history[currentHistoryNumber - 1], 0, 0, 0, 0, canvas);
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
        app->drawOnScreen(lastSavedDC);
    }  */

    //history[addNewHistoryNum].toolsNum = _app->systemSettings->DrawingMode;
    //history[addNewHistoryNum].pos = {lastClick.x + canvasCoordinats.x,  lastClick.y + canvasCoordinats.y};
    //history[addNewHistoryNum].size = size;
    //history[addNewHistoryNum].color = _app->systemSettings->DrawColor;
    //history[addNewHistoryNum].thickness = lineThickness;



}

void Canvas::deleteHistory()
{
    for (int i = 0; i < HistoryLength; i++)
    {
        //smartDeleteDC (history[i]);
    }
}

void Canvas::controlEditLay()
{
    if (!getActiveLay()) return;
    ToolLay* activeToolLay = getActiveLay()->getActiveToolLay();
    if (app->getAsyncKeyState('E') && currentToolLength > 0 && activeToolLay->isFinished())
    {
        while (app->getAsyncKeyState('E')) {};
        editingMode = !editingMode;
        activeToolLay->needRedraw();
    }
}

bool Canvas::controlLay()
{
    /*
    ToolLay* activeToolLay = getActiveLay()->getActiveToolLay();
    if (app->getAsyncKeyState(VK_RIGHT) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {1, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState(VK_LEFT) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {-1, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState(VK_DOWN) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {0, 1};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState(VK_UP) && currentToolLength > 0)
    {
        activeToolLay->toolZone.pos += {0, -1};
        activeToolLay->needRedraw();
    }

    if (app->getAsyncKeyState('T') && currentToolLength > 0)
    {
        activeToolLay->size += { 0.01, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState('G') && currentToolLength > 0)
    {
        activeToolLay->size += {-0.01, 0};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState('Y') && currentToolLength > 0)
    {
        activeToolLay->size += { 0, 0.01};
        activeToolLay->needRedraw();
    }
    if (app->getAsyncKeyState('H') && currentToolLength > 0)
    {
        activeToolLay->size += { 0, -0.01};
        activeToolLay->needRedraw();
    }
    */
    return false;
}

void Canvas::cleanOutputLay()
{
    for (int i = 0; i < currentLayersLength; i++)
    {
        //if (lay[i].redrawStatus ()) app->bitBlt(lay[i].getOutPutDC(), 0, 0, 0, 0, lay[i].getPermanentDC());
        //txAlphaBlend(lay[i].outputLay, 0, 0, 0, 0, lay[i].lay);
        //lay[i].clean(lay[i].outputLay);
        //txTransparentBlt (lay[i].outputLay, 0, 0, 0, 0, lay[i].lay, 0, 0, _app->systemSettings->TRANSPARENTCOLOR);
        //app->bitBlt(0, 0, lay[i].tempLay);
        //while (app->getAsyncKeyState('G')) { txSleep(0); }

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
        }

        if (editingMode && (lays == getActiveLayNum()))
        {
            //lay[lays].getActiveToolLay()->setMBCondition(getMBCondition());
            lay[lays].editTool(currentDate);
        }

        app->transparentBlt(finalDC, lay[lays].lay.layCoordinats.x, lay[lays].lay.layCoordinats.y, 0, 0, lay[lays].lay.outputLay);
    }
}


void Canvas::drawCadre()
{
    zoneSizeControl.drawFrame();

}