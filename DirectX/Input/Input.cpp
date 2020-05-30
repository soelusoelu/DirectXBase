#include "Input.h"
#include "JoyPad.h"
#include "Keyboard.h"
#include "Math.h"
#include "Mouse.h"
#include "../System/Game.h"

bool Input::initialize(HWND hWnd) {
    // 「DirectInput」オブジェクトの作成
    if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&mDirectInput, NULL))) {
        return false;
    }

    mKeyboard = new Keyboard();
    mKeyboard->initialize(hWnd, mDirectInput);

    mMouse = new Mouse();
    mMouse->initialize(hWnd, mDirectInput);

    mJoyPad = new JoyPad();
    mJoyPad->initialize(hWnd, mDirectInput);

    return true;
}

void Input::loadProperties(const rapidjson::Value& inObj) {
    mKeyboard->loadProperties(inObj);
    mJoyPad->loadProperties(inObj);
}

void Input::end() {
    SAFE_RELEASE(mDirectInput);
    SAFE_DELETE(mKeyboard);
    SAFE_DELETE(mMouse);
    SAFE_DELETE(mJoyPad);
}

void Input::update() {
    mKeyboard->update();
    mMouse->update();
    mJoyPad->update();
}

Keyboard* Input::keyboard() {
    return mKeyboard;
}

Mouse* Input::mouse() {
    return mMouse;
}

JoyPad* Input::joyPad() {
    return mJoyPad;
}

IDirectInput8* Input::mDirectInput = nullptr;
Keyboard* Input::mKeyboard = nullptr;
Mouse* Input::mMouse = nullptr;
JoyPad* Input::mJoyPad = nullptr;
