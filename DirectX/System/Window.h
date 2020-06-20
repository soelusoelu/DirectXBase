#pragma once

#include "SystemInclude.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>

#define TITLE L"からあげ"

class Window {
public:
    Window();
    ~Window();
    HRESULT initWindow(HINSTANCE, INT, INT);
    LRESULT msgProc(HWND, UINT, WPARAM, LPARAM);
    HWND gethWnd() const;
    void update();
    static int width();
    static int height();
    static int standardWidth();
    static int standardHeight();
    static int debugWidth();
    static int debugHeight();
    static Vector2 windowToClientSize();
    void loadProperties(const rapidjson::Value& inObj);

private:
    void updateWindowToClientSize();

private:
    HWND mhWnd;
    static int mWidth;
    static int mHeight;
    static int mStandardWidth;
    static int mStandardHeight;
    static int mDebugWidth;
    static int mDebugHeight;
    static Vector2 mWindowToClientSize;
};

