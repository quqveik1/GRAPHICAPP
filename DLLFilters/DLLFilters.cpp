// DLLFilters.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "DLLFilters.h"


// This is an example of an exported variable
DLLFILTERS_API int nDLLFilters=0;

// This is an example of an exported function.
DLLFILTERS_API int fnDLLFilters(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CDLLFilters::CDLLFilters()
{
    return;
}
