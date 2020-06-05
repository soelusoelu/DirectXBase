#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "JoyPad.h"

class Input {
public:
    static Keyboard* keyboard();
    static Mouse* mouse();
    static JoyPad* joyPad();

private:
    //生成、コピー禁止
    Input() = delete;
    ~Input() = delete;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
};
