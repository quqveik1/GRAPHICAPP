#include "WindowsLibApi.h"
#include "DrawBibliothek.cpp"

void WindowsLibApi::resize(Window* window, Rect newRect)
{
    if (window->systemSettings->debugMode >= 2) printf("newRect {%lf, %lf}; {%lf, %lf}\n", newRect.pos.x, newRect.pos.y, newRect.finishPos.x, newRect.finishPos.y);
    if (newRect.getSize().x > 0 && newRect.getSize().y > 0)
    {
        window->finalDCSize = { newRect.getSize().x, newRect.getSize().y };
        if (window->systemSettings->debugMode >= 2) printf("finalDCSize {%lf, %lf}; \n", window->finalDCSize.x, window->finalDCSize.y);

        window->app->deleteDC(window->finalDC);
        window->finalDC = window->app->createDIBSection(window->finalDCSize, &window->finalDCArr);

        window->app->setColor(window->color, window->finalDC);
        window->app->rectangle(0, 0, newRect.getSize().x, newRect.getSize().y, window->finalDC);
        if (window->systemSettings->debugMode == 5) window->app->drawOnScreen(window->finalDC);
    }
    window->rect = newRect;
}



bool WindowsLibApi::addWindow(Manager* manager, Window* window)
{
    if (!window)
    {
        printf("������� �������� �������������� ����\n");
        return 0;
    }
    if (manager->currLen >= manager->length)
    {
        printf("!!!Unable to add new Window!!!\n");
        return 0;
    }

    manager->pointers[manager->currLen] = window;
    manager->currLen++;

    window->manager = manager;

    return 1;
}


bool WindowsLibApi::clickHandle(Manager* manager)
{
    if (manager->handle.rect.inRect(manager->getMousePos()))
    {
        manager->startCursorPos.x = manager->getAbsMousePos().x;
        manager->startCursorPos.y = manager->getAbsMousePos().y;
        manager->handle.setMbLastTime();
        return true;
    }
    return false;
}


Vector WindowsLibApi::getAbsCoordinats(Window* window, bool coordinatsWithHandle /*=false*/)
{

    Vector coordinats = {};

    Manager* copyOfManager = window->manager;

    coordinats += window->rect.pos;

    for (;;)
    {
        if (!copyOfManager) break;

        coordinats = coordinats + copyOfManager->rect.pos;
        if (copyOfManager->coordinatSysFromHandle && !coordinatsWithHandle) coordinats.y += copyOfManager->handle.rect.finishPos.y;
        copyOfManager = copyOfManager->manager;

    }


    return coordinats;
}


Window* WindowsLibApi::isActiveWindowBelow(Manager* manager)
{
    if (manager->getActiveWindow() == manager) return manager;
    for (int i = 0; i < manager->getCurLen(); i++)
    {
        Window* activeWindowBelow = NULL;
        if (manager->pointers[i]) activeWindowBelow = manager->pointers[i]->isActiveWindowBelow();
        if (activeWindowBelow) return activeWindowBelow;
    }
    return NULL;
}


void WindowsLibApi::controlHandle(Manager* manager)
{
    bool isClickedAgo = manager->handle.isClickedLastTime();
    if (manager->app->systemSettings->debugMode == 5) printf("isClickedLastTime: %d\n", isClickedAgo);

    Vector absMP = manager->getAbsMousePos();

    if (manager->app->systemSettings->debugMode == 5) printf("absMP: {%lf, %lf}\n", absMP.x, absMP.y);

    if (isClickedAgo && manager)
    {
        manager->rect.pos.x += absMP.x - manager->startCursorPos.x;
        manager->rect.pos.y += absMP.y - manager->startCursorPos.y;
        manager->rect.finishPos.x += absMP.x - manager->startCursorPos.x;
        manager->rect.finishPos.y += absMP.y - manager->startCursorPos.y;
        manager->startCursorPos.x = absMP.x;
        manager->startCursorPos.y = absMP.y;
    }
    if (manager->getMBCondition() == 0) manager->handle.setMbLastTime();
}


void WindowsLibApi::screenChanged(Manager* manager)
{
    for (int i = 0; i < manager->getCurLen(); i++)
    {
        if (manager->pointers[i]) manager->pointers[i]->screenChanged();
    }
}

int WindowsLibApi::standartWindowDraw(struct Window* window)
{
    standartDraw$(window);
    return 0;
}

int WindowsLibApi::standartManagerDraw(Manager* manager)
{
    standartManagerDraw$(manager);
    return 0;
}


int WindowsLibApi::standartManagerOnClick(Manager* manager, Vector mp)
{
    return standartManagerOnClick$(manager, mp);
}