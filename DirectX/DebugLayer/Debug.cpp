#include "Debug.h"
#include "FixedDebugInformation.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "Log.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"

void Debug::create() {
    mDrawString = new DrawString();
    mLog = new Log();
    mFixedDebugInfo = new FixedDebugInformation(mDrawString);
    mHierarchy = new Hierarchy(mDrawString);
    mInspector = new Inspector(mDrawString);
}

void Debug::loadProperties(const rapidjson::Value& inObj) {
    mDrawString->loadProperties(inObj);
    mLog->loadProperties(inObj);
    mFixedDebugInfo->loadProperties(inObj);
    mHierarchy->loadProperties(inObj);
    mInspector->loadProperties(inObj);
}

void Debug::initialize(std::shared_ptr<Renderer> renderer) {
    mDrawString->initialize(renderer);
    mLog->initialize();
    mFixedDebugInfo->initialize();
    mHierarchy->initialize();
    mInspector->initialize();
}

void Debug::finalize() {
    SAFE_DELETE(mInspector);
    SAFE_DELETE(mHierarchy);
    SAFE_DELETE(mFixedDebugInfo);
    SAFE_DELETE(mLog);
    SAFE_DELETE(mDrawString);
}

void Debug::update() {
    mLog->update();
}

void Debug::windowMessage(const std::string& message) {
#ifdef _DEBUG
    MessageBoxA(0, message.c_str(), 0, MB_OK);
#endif // _DEBUG
}

void Debug::draw(const Matrix4& proj) {
#ifdef _DEBUG
    mLog->drawLogs(mDrawString);
    mHierarchy->drawActors();
    mInspector->drawInspect();
    mDrawString->drawAll(proj);
#endif // _DEBUG
}

void Debug::drawStringClear() {
    mDrawString->clear();
}

Log* Debug::log() {
    return mLog;
}

FixedDebugInformation* Debug::fixedDebugInfo() {
    return mFixedDebugInfo;
}

Hierarchy* Debug::hierarchy() {
    return mHierarchy;
}

Inspector* Debug::inspector() {
    return mInspector;
}

DrawString* Debug::mDrawString = nullptr;
Log* Debug::mLog = nullptr;
FixedDebugInformation* Debug::mFixedDebugInfo = nullptr;
Hierarchy* Debug::mHierarchy = nullptr;
Inspector* Debug::mInspector = nullptr;
