// DLLTools.cpp : Defines the exported functions for the DLL.
//
#pragma once
#include "pch.h"
#include "framework.h"
#include "DLLTools.h"


// This is an example of an exported variable
DLLTOOLS_API int nDLLTools=0;

// This is an example of an exported function.
DLLTOOLS_API int fnDLLTools(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CDLLTools::CDLLTools()
{
    return;
}
