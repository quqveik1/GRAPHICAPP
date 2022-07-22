#pragma once
#include "pch.h"
#include "..\Q_Vector.h"
#include "atlimage.h"  


extern "C" __declspec (dllexport) int saveImage(HDC dc, const char* path);