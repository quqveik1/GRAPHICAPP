#pragma once
#include "pch.h"
#include "..\Q_Vector.h"
#include "atlimage.h"  
#include <string>


extern "C" __declspec (dllexport) int saveImage(HDC dc, const char* path);