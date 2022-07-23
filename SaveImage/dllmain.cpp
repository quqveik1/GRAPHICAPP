// dllmain.cpp : Defines the entry point for the DLL application.  
#include "dllmain.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int saveImage(HDC dc, const char* path)
{
    HBITMAP bitmap = (HBITMAP)GetCurrentObject(dc, OBJ_BITMAP);

    CImage image;
    image.Attach(bitmap);

    wchar_t* vOut = new wchar_t[strlen(path) + 1];
    mbstowcs_s(NULL, vOut, strlen(path) + 1, path, strlen(path));
    LPCTSTR widePath = vOut;
    HRESULT hresult = image.Save(widePath);
    delete[] vOut;

    if (FAILED(hresult))
    {
        return hresult;
    }

    return 0;
}

HDC loadImage(const char* path, Vector& size)
{
    CImage image;
    wchar_t* vOut = new wchar_t[strlen(path) + 1];
    mbstowcs_s(NULL, vOut, strlen(path) + 1, path, strlen(path));
    LPCTSTR widePath = vOut;
    HRESULT hresult = image.Load(widePath);

    size.x = image.GetWidth();
    size.y = image.GetHeight();
    return image.GetDC();
}

