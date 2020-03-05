#include "Debug.h"
#include "../System/DirectXIncLib.h"

void Debug::windowMessage(const std::string& message) {
#ifdef _DEBUG
    MessageBoxA(0, message.c_str(), 0, MB_OK);
#endif // _DEBUG
}
