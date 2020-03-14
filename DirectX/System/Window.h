#pragma once

#include "DirectXIncLib.h"
#include <rapidjson/document.h>

#define TITLE L"DirectX"

class Window {
public:
    HRESULT initWindow(HINSTANCE, INT, INT);
    LRESULT msgProc(HWND, UINT, WPARAM, LPARAM);
    HWND gethWnd() const;
    static int width();
    static int height();
    static int debugWidth();
    static int debugHeight();
    void loadProperties(const rapidjson::Value& inObj);

private:
    HWND mhWnd;
    static int mWidth;
    static int mHeight;
    static int mDebugWidth;
    static int mDebugHeight;
};

