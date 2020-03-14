#include "Debug.h"
#include "../Device/FixedDebugInformation.h"
#include "../Device/Inspector.h"
#include "../Device/Log.h"
#include "../Device/Renderer.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"

void Debug::loadProperties(const rapidjson::Value& inObj) {
    //あまり良くない書き方
    mLog->loadProperties(inObj);
    mFixedDebugInfo->loadProperties(inObj);
    mInspector->loadProperties(inObj);

    mLog->initialize();
    mFixedDebugInfo->initialize();
    mInspector->initialize();
}

void Debug::initialize(std::shared_ptr<Renderer> renderer) {
    mLog = new Log(renderer->getDrawString());
    mFixedDebugInfo = new FixedDebugInformation(renderer->getDrawString());
    mInspector = new Inspector(renderer->getDrawString());
}

void Debug::finalize() {
    SAFE_DELETE(mLog);
    SAFE_DELETE(mFixedDebugInfo);
    SAFE_DELETE(mInspector);
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

Inspector* Debug::inspector() {
    return mInspector;
}

Log* Debug::mLog = nullptr;
FixedDebugInformation* Debug::mFixedDebugInfo = nullptr;
Inspector* Debug::mInspector = nullptr;
