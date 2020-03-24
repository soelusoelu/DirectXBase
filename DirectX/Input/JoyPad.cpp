﻿#include "JoyPad.h"
#include "Input.h"
#include "../System/Game.h"

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);

JoyPad::JoyPad() :
    mCurrentJoyState(),
    mPreviousJoyState() {
}

JoyPad::~JoyPad() {
    SAFE_RELEASE(mPadDevice);
}

bool JoyPad::initialize(HWND hWnd, LPDIRECTINPUT8 directInput) {
    //利用可能なゲームコントローラーの列挙関数を実行
    if (FAILED(directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
        return false;
    }
    if (!mPadDevice) { //パッドの接続はせずに終了
        return true;
    }
    // デバイスをジョイスティックに設定
    if (FAILED(mPadDevice->SetDataFormat(&c_dfDIJoystick2))) {
        return false;
    }
    // 協調レベルの設定
    if (FAILED(mPadDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        return false;
    }
    //アプリケーションで使用するコントローラーのプロパティを列挙して設定する
    if (FAILED(mPadDevice->EnumObjects(enumObjectsCallback, NULL, DIDFT_ALL))) {
        return false;
    }
    // デバイスを「取得」する
    mPadDevice->Acquire();

    return true;
}

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
    // 「DirectInputデバイス」オブジェクトの作成
    if (FAILED(Input::mDirectInput->CreateDevice(pdidInstance->guidInstance, &JoyPad::mPadDevice, NULL))) {
        return DIENUM_CONTINUE;
    }

    return DIENUM_STOP;
}

BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
    if (pdidoi->dwType & DIDFT_AXIS) {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof(DIPROPRANGE);
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType;
        diprg.lMin = -JoyPad::STICK_VALUE;
        diprg.lMax = JoyPad::STICK_VALUE;

        if (FAILED(JoyPad::mPadDevice->SetProperty(DIPROP_RANGE, &diprg.diph))) {
            return DIENUM_STOP;
        }
    }
    return DIENUM_CONTINUE;
}

void JoyPad::update() {
    if (mPadDevice) {
        mPreviousJoyState = mCurrentJoyState;

        HRESULT hr = mPadDevice->Acquire();
        if ((hr == DI_OK) || (hr == S_FALSE)) {
            mPadDevice->GetDeviceState(sizeof(DIJOYSTATE2), &mCurrentJoyState);
        }
    }
}

bool JoyPad::getJoyDown(JoyCode joy) const {
    return (mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80 && !(mPreviousJoyState.rgbButtons[static_cast<int>(joy)] & 0x80));
}

bool JoyPad::getJoy(JoyCode joy) const {
    return mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80;
}

bool JoyPad::getJoyUp(JoyCode joy) const {
    return (!(mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80) && mPreviousJoyState.rgbButtons[static_cast<int>(joy)] & 0x80);
}

Vector2 JoyPad::leftStick() const {
    Vector2 temp = Vector2::zero;
    temp.x = (Math::abs(mCurrentJoyState.lX) > DEAD_ZONE) ? mCurrentJoyState.lX / STICK_VALUE : 0.f;
    temp.y = (Math::abs(mCurrentJoyState.lY) > DEAD_ZONE) ? -mCurrentJoyState.lY / STICK_VALUE : 0.f;

    return temp;
}

Vector2 JoyPad::rightStick() const {
    Vector2 temp = Vector2::zero;
    temp.x = (Math::abs(mCurrentJoyState.lRx) > DEAD_ZONE) ? mCurrentJoyState.lRx / STICK_VALUE : 0.f;
    temp.y = (Math::abs(mCurrentJoyState.lRx) > DEAD_ZONE) ? -mCurrentJoyState.lRx / STICK_VALUE : 0.f;

    return temp;
}

LPDIRECTINPUTDEVICE8 JoyPad::mPadDevice = nullptr;