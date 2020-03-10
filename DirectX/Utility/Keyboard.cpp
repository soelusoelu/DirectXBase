#include "Keyboard.h"
#include "../System/Game.h"

Keyboard::Keyboard() :
    mKeyDevice(nullptr),
    mCurrentKeys(),
    mPreviousKeys() {
}

Keyboard::~Keyboard() {
    SAFE_RELEASE(mKeyDevice);
}

bool Keyboard::initialize(HWND hWnd, LPDIRECTINPUT8 directInput) {
    // 「DirectInputデバイス」オブジェクトの作成
    if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &mKeyDevice, NULL))) {
        return false;
    }
    // デバイスをキーボードに設定
    if (FAILED(mKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
        return false;
    }
    // 協調レベルの設定
    if (FAILED(mKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        return false;
    }
    // デバイスを「取得」する
    mKeyDevice->Acquire();

    return true;
}

void Keyboard::update() {
    memcpy_s(mPreviousKeys, sizeof(mPreviousKeys), mCurrentKeys, sizeof(mCurrentKeys));

    HRESULT hr = mKeyDevice->Acquire();
    if ((hr == DI_OK) || (hr == S_FALSE)) {
        mKeyDevice->GetDeviceState(sizeof(mCurrentKeys), &mCurrentKeys);
    }
}

bool Keyboard::getKeyDown(KeyCode key) {
    return (mCurrentKeys[static_cast<BYTE>(key)] & 0x80 && !(mPreviousKeys[static_cast<BYTE>(key)] & 0x80));
}

bool Keyboard::getKey(KeyCode key) {
    return mCurrentKeys[static_cast<BYTE>(key)] & 0x80;
}

bool Keyboard::getKeyUp(KeyCode key) {
    return (!(mCurrentKeys[static_cast<BYTE>(key)] & 0x80) && mPreviousKeys[static_cast<BYTE>(key)] & 0x80);
}

int Keyboard::horizontal() {
    if (getKey(KeyCode::A) || getKey(KeyCode::LeftArrow)) {
        return -1;
    } else if (getKey(KeyCode::D) || getKey(KeyCode::RightArrow)) {
        return 1;
    } else {
        return 0;
    }
}

int Keyboard::vertical() {
    if (getKey(KeyCode::W) || getKey(KeyCode::UpArrow)) {
        return 1;
    } else if (getKey(KeyCode::S) || getKey(KeyCode::DownArrow)) {
        return -1;
    } else {
        return 0;
    }
}
