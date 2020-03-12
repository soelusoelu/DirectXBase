#include "Debug.h"
#include "../Device/FixedDebugInformation.h"
#include "../Device/Log.h"
#include "../Device/Renderer.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"

void Debug::initialize(std::shared_ptr<Renderer> renderer) {
    mLog = new Log(renderer->getDrawString());
    mFixedDebugInfo = new FixedDebugInformation(renderer->getDrawString());
}

void Debug::finalize() {
    SAFE_DELETE(mLog);
    SAFE_DELETE(mFixedDebugInfo);
}

void Debug::windowMessage(const std::string& message) {
#ifdef _DEBUG
    MessageBoxA(0, message.c_str(), 0, MB_OK);
#endif // _DEBUG
}

Log* Debug::log() {
    return mLog;
}

FixedDebugInformation* Debug::fixedDebugInfo() {
    return mFixedDebugInfo;
}

Log* Debug::mLog = nullptr;
FixedDebugInformation* Debug::mFixedDebugInfo = nullptr;
