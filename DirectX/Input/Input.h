#pragma once

#include "Math.h"
#include "../System/DirectXIncLib.h"
#include <dinput.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")

class JoyPad;
class Keyboard;
class Mouse;

class Input {
public:
    static bool initialize(HWND hWnd);
    static void end();
    static void update();

    static Keyboard* keyboard();
    static Mouse* mouse();
    static JoyPad* joyPad();

public:
    static LPDIRECTINPUT8 mDirectInput;

private:
    static Keyboard* mKeyboard;
    static Mouse* mMouse;
    static JoyPad* mJoyPad;
};
