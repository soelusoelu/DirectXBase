#include "Window.h"
#include "../Utility/LevelLoader.h"

Window* gWindow = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return gWindow->msgProc(hWnd, uMsg, wParam, lParam);
}

HRESULT Window::initWindow(HINSTANCE hInstance, INT iX, INT iY) {
    gWindow = this;
    // ウィンドウの定義
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = TITLE;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);
    //ウィンドウの作成
#ifdef _DEBUG
    mhWnd = CreateWindow(TITLE, TITLE, WS_OVERLAPPEDWINDOW, iX, iY, mDebugWidth, mDebugHeight, 0, 0, hInstance, 0);
#else
    mhWnd = CreateWindow(TITLE, TITLE, WS_OVERLAPPEDWINDOW, iX, iY, mWidth, mHeight, 0, 0, hInstance, 0);
#endif // _DEBUG

    if (!mhWnd) {
        return E_FAIL;
    }

    //ウインドウの表示
    ShowWindow(mhWnd, SW_SHOW);
    UpdateWindow(mhWnd);

    return S_OK;
}

LRESULT Window::msgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch (iMsg) {
    case WM_KEYDOWN:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

HWND Window::gethWnd() const {
    return mhWnd;
}

void Window::update() {
    updateWindowToClientSize();
}

int Window::width() {
    return mWidth;
}

int Window::height() {
    return mHeight;
}

int Window::debugWidth() {
    return mDebugWidth;
}

int Window::debugHeight() {
    return mDebugHeight;
}

Vector2 Window::windowToClientSize() {
    return mWindowToClientSize;
}

void Window::loadProperties(const rapidjson::Value& inObj) {
    const auto& windowObj = inObj["window"];
    if (windowObj.IsObject()) {
#ifdef _DEBUG
        JsonHelper::getInt(windowObj, "windowWidth", &mWidth);
        JsonHelper::getInt(windowObj, "windowHeight", &mHeight);
#else
        JsonHelper::getInt(windowObj, "releaseWindowWidth", &mWidth);
        JsonHelper::getInt(windowObj, "releaseWindowHeight", &mHeight);
#endif // _DEBUG
        JsonHelper::getInt(windowObj, "windowDebugWidth", &mDebugWidth);
        JsonHelper::getInt(windowObj, "windowDebugHeight", &mDebugHeight);
    }
}

void Window::updateWindowToClientSize() {
    RECT wRect, cRect; //ウィンドウ全体の矩形、クライアント領域の矩形
    int ww, wh;        //ウィンドウ全体の幅、高さ
    int cw, ch;        //クライアント領域の幅、高さ //ウィンドウ全体の幅・高さを計算

    //ウィンドウ全体の幅・高さを計算
    //GetWindowRect(mhWnd, &wRect);
    //ww = wRect.right - wRect.left;
    //wh = wRect.bottom - wRect.top;
    //クライアント領域の幅・高さを計算
    GetClientRect(mhWnd, &cRect);
    cw = cRect.right - cRect.left;
    ch = cRect.bottom - cRect.top;
    //ウィンドウとクライアントの比率を計算
#ifdef _DEBUG
    mWindowToClientSize.x = static_cast<float>(mDebugWidth) / static_cast<float>(cw);
    mWindowToClientSize.y = static_cast<float>(mDebugHeight) / static_cast<float>(ch);
#else
    mWindowToClientSize.x = static_cast<float>(mWidth) / static_cast<float>(cw);
    mWindowToClientSize.y = static_cast<float>(mHeight) / static_cast<float>(ch);
#endif // _DEBUG
}

int Window::mWidth = 1080;
int Window::mHeight = 720;
int Window::mDebugWidth = 1500;
int Window::mDebugHeight = 1000;
Vector2 Window::mWindowToClientSize = Vector2::one;
