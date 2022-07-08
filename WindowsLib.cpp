#pragma once
#include "WindowsLib.h"


bool Manager::addWindow(Window* window)
{
    if (!window)
    {
        printf("ѕопытка добавить несуществующее окно\n");
        return 0;
    }
    if (currLen >= length)
    {
        printf("!!!Unable to add new Window!!!\n");
        return 0;
    }

    pointers[currLen] = window;
    currLen++;

    window->manager = this;

    return 1;
}


void Window::print(HDC DC)
{
    assert(DC);
    draw();
    app->bitBlt(DC, rect.pos.x, rect.pos.y, rect.getSize().x, rect.getSize().y, finalDC);
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
    rect = rect + delta;
}

void Window::draw()
{
    app->standartWindowDraw(this);
}

void Window::resize(Rect newRect)
{

    if (systemSettings->debugMode == 2) printf("newRect {%lf, %lf}; {%lf, %lf}\n", newRect.pos.x, newRect.pos.y, newRect.finishPos.x, newRect.finishPos.y);
    if (newRect.getSize().x > 0 && newRect.getSize().y > 0)
    {
        finalDCSize = { newRect.getSize().x, newRect.getSize().y };
        if (systemSettings->debugMode == 2) printf("finalDCSize {%lf, %lf}; \n", finalDCSize.x, finalDCSize.y);

        app->deleteDC(finalDC);
        finalDC = app->createDIBSection(finalDCSize, &finalDCArr);

        app->setColor(color, finalDC);
        app->rectangle(0, 0, newRect.getSize().x, newRect.getSize().y, finalDC);
        if (systemSettings->debugMode == 5) app->drawOnScreen(finalDC);
    }
    rect = newRect;
}

void Window::reInit()
{
    if (rect.getSize().x > 0 && rect.getSize().y > 0)
    {
        finalDCSize = { rect.getSize().x, rect.getSize().y };
        finalDC = app->createDIBSection(finalDCSize.x, finalDCSize.y, &finalDCArr);
        app->setColor(color, finalDC);
        app->rectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);
        if (systemSettings->debugMode == 5) app->drawOnScreen(finalDC);
    }

    originalRect = rect;
}

void Window::setStartRect(Vector pos, Vector finishPos)
{
    assert(pos > 0 && finishPos > 0);
    rect = { pos, finishPos };
    originalRect = rect;
}
Vector Window::getRelativeMousePos(bool coordinatsWithHandle)
{
    //POINT mousePos = {(double)txMouseX() - getAbsCoordinats(coordinatsWithHandle).x, (double) txMouseY () - getAbsCoordinats(coordinatsWithHandle).y};
    //ScreenToClient (systemSettings->MAINWINDOW, &mousePos);
    //return {(double)mousePos.x, (double)mousePos.y};
    return { 0, 0 };
}

Vector Window::getAbsCoordinats(bool coordinatsWithHandle /*=false*/)
{
    Vector coordinats = {};

    Manager* copyOfManager = manager;

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

Rect Window::getAbsRect(bool coordinatsWithHandle /*=false*/)
{
    Rect coordinats = {};

    Manager* copyOfManager = manager;

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



Window* Manager::isActiveWindowBelow()
{
    if (getActiveWindow() == this) return this;
    for (int i = 0; i < getCurLen(); i++)
    {
        Window* activeWindowBelow = NULL;
        if (pointers[i]) activeWindowBelow = pointers[i]->isActiveWindowBelow();
        if (activeWindowBelow) return activeWindowBelow;
    }
    return NULL;
}


void Manager::draw()
{
    app->standartManagerDraw(this);
}

bool Manager::clickHandle()
{
    if (handle.rect.inRect(getMousePos()))
    {
        startCursorPos.x = getAbsMousePos().x;
        startCursorPos.y = getAbsMousePos().y;
        handle.setMbLastTime();
        return true;
    }
    return false;
}

void Manager::controlHandle()
{
    bool isClickedAgo = handle.isClickedLastTime();
    if (app->systemSettings->debugMode == 5) printf("isClickedLastTime: %d\n", isClickedAgo);

    Vector absMP = getAbsMousePos();

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
    if (getMBCondition() == 0) handle.setMbLastTime();
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

void Manager::onClick(Vector mp)
{
    app->standartManagerOnClick(this, mp);
}




void Manager::defaultDestructor()
{
    assert(app);
    if (dc) app->smartDeleteDC(dc);
    if (finalDC) app->smartDeleteDC(finalDC);
    for (int i = 0; i < getCurLen(); i++)
    {
        if (pointers[i]) delete pointers[i];
    }
};

void Window::defaultDestructor()
{
    assert(app);
    if (dc) app->smartDeleteDC(dc);
    if (finalDC) app->smartDeleteDC(dc);
}

void Manager::hide()
{
    needToShow = false;
    for (int i = 0; i < getCurLen(); i++)
    {
        pointers[i]->hide();
    }
}

void Manager::show()
{
    needToShow = true;
    for (int i = 0; i < getCurLen(); i++)
    {
        pointers[i]->show();
    }
}

void Manager::screenChanged()
{
    for (int i = 0; i < getCurLen(); i++)
    {
        if (pointers[i]) pointers[i]->screenChanged();
    }
}