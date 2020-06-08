#include "Mouse.h"
#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
#include "../System/Window.h"

Mouse::Mouse() :
    mMouseDevice(nullptr),
    mCurrentMouseState(),
    mPreviousMouseState(),
    mhWnd(nullptr),
    mMousePositionX(0.f),
    mMousePositionY(0.f) {
}

Mouse::~Mouse() {
    safeRelease<IDirectInputDevice8>(mMouseDevice);
}

bool Mouse::getMouseButtonDown(MouseCode button) {
    return (mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80 && !(mPreviousMouseState.rgbButtons[static_cast<int>(button)] & 0x80));
}

bool Mouse::getMouseButton(MouseCode button) {
    return mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80;
}

bool Mouse::getMouseButtonUp(MouseCode button) {
    return (!(mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80) && mPreviousMouseState.rgbButtons[static_cast<int>(button)] & 0x80);
}

Vector2 Mouse::getMousePosition() {
    return Vector2(mMousePositionX, mMousePositionY);
}

bool Mouse::initialize(HWND hWnd, IDirectInput8* directInput) {
    mhWnd = hWnd;

    // 「DirectInputデバイス」オブジェクトの作成
    if (FAILED(directInput->CreateDevice(GUID_SysMouse, &mMouseDevice, NULL))) {
        return false;
    }
    // デバイスをマウスに設定
    if (FAILED(mMouseDevice->SetDataFormat(&c_dfDIMouse2))) {
        return false;
    }
    // 協調レベルの設定
    if (FAILED(mMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        return false;
    }
    // デバイスを「取得」する
    mMouseDevice->Acquire();

    return true;
}

void Mouse::update() {
    mPreviousMouseState = mCurrentMouseState;

    HRESULT hr = mMouseDevice->Acquire();
    if ((hr == DI_OK) || (hr == S_FALSE)) {
        mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mCurrentMouseState);
    }

    //カーソル座標更新
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(mhWnd, &point);
    mMousePositionX = point.x;
    mMousePositionY = point.y;
    mMousePositionX *= Window::windowToClientSize().x;
    mMousePositionY *= Window::windowToClientSize().y;

    //マウス座標をウィンドウ幅でクランプ
#ifdef _DEBUG
    mMousePositionX = Math::clamp<float>(mMousePositionX, 0.f, static_cast<float>(Window::debugWidth()));
    mMousePositionY = Math::clamp<float>(mMousePositionY, 0.f, static_cast<float>(Window::debugHeight()));
#else
    mMousePositionX = Math::clamp<float>(mMousePositionX, 0.f, static_cast<float>(Window::width()));
    mMousePositionY = Math::clamp<float>(mMousePositionY, 0.f, static_cast<float>(Window::height()));
#endif // _DEBUG
}

void Mouse::stringToJoyCode(const std::string& src, MouseCode* dst) {
    auto key = MouseCode::None;

    if (src == "LeftButton") {
        key = MouseCode::LeftButton;
    } else if (src == "RightButton") {
        key = MouseCode::RightButton;
    } else if (src == "CenterButton") {
        key = MouseCode::CenterButton;
    } else if (src == "SideButton1") {
        key = MouseCode::SideButton1;
    } else if (src == "SideButton2") {
        key = MouseCode::SideButton2;
    } else if (src == "SideButton3") {
        key = MouseCode::SideButton3;
    }

    if (key != MouseCode::None) {
        *dst = key;
    }
}
