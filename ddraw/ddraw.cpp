#include "DirectDraw.hpp"

#include "glrage.h"
#include "Logger.hpp"

#include <string>

using ddraw::DirectDraw;

HRESULT HandleException() {
    try {
        throw;
    } catch (const std::exception& ex) {
        MessageBox(GLRageGetHWnd(), ex.what(), nullptr, MB_OK | MB_ICONERROR);
        return DDERR_GENERIC;
    }
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
    TRACEF("DirectDraw DllMain(%p,%d)",hModule, dwReason); 
    return TRUE;
}

HRESULT WINAPI DirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter) {
    TRACEF("DirectDrawCreate");

    GLRageInit();
    GLRageAttach();

    try {
        *lplpDD = new DirectDraw();
    } catch (...) {
        return HandleException();
    }

    return DD_OK;
}