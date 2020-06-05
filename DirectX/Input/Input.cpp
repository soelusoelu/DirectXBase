#include "Input.h"
#include "InputUtility.h"

Keyboard* Input::keyboard() {
    return InputUtility::keyboard();
}

Mouse* Input::mouse() {
    return InputUtility::mouse();
}

JoyPad* Input::joyPad() {
    return InputUtility::joyPad();
}
