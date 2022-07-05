#pragma once
#include "OpenManager.cpp"

struct OpenHandleMenuManager : OpenManager
{
    OpenHandleMenuManager(AbstractAppData* _app, HDC _dc) :
        OpenManager(_app, {}, _app->systemSettings->MenuColor, NULL, _dc)
    {

    }

    virtual void showControl() override;

};