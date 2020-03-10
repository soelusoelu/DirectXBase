#include "Mouse.h"
#include "../System/Game.h"

Mouse::Mouse() :
    mCurrentMouseState(),
    mPreviousMouseState(),
    mMouseDevice(nullptr),
    mMousePosition(Vector2::zero),
    mhWnd(nullptr) {
}

Mouse::~Mouse() {
    SAFE_RELEASE(mMouseDevice);
}

bool Mouse::initialize(HWND hWnd, LPDIRECTINPUT8 directInput) {
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
    mMousePosition.x = point.x;
    mMousePosition.y = point.y;

    //マウス座標をウィンドウ幅でクランプ
    mMousePosition.clamp(Vector2::zero, Vector2(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT));
}

bool Mouse::getMouseDown(MouseCode button) {
    return (mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80 && !(mPreviousMouseState.rgbButtons[static_cast<int>(button)] & 0x80));
}

bool Mouse::getMouse(MouseCode button) {
    return mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80;
}

bool Mouse::getMouseUp(MouseCode button) {
    return (!(mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80) && mPreviousMouseState.rgbButtons[static_cast<int>(button)] & 0x80);
}

const Vector2& Mouse::getMousePosition() {
    return mMousePosition;
}
