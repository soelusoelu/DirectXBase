#include "Debug.h"
#include "../Device/Log.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"

void Debug::initialize(std::shared_ptr<Renderer> renderer) {
    mLog = new Log(renderer);
}

void Debug::finalize() {
    SAFE_DELETE(mLog);
}

void Debug::windowMessage(const std::string& message) {
#ifdef _DEBUG
    MessageBoxA(0, message.c_str(), 0, MB_OK);
#endif // _DEBUG
}

Log* Debug::log() {
    return mLog;
}

Log* Debug::mLog = nullptr;
