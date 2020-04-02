#include "DebugUtility.h"
#include "FixedDebugInformation.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "Log.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"

void DebugUtility::create() {
    mDrawString = new DrawString();
    mLog = new Log();
    mFixedDebugInfo = new FixedDebugInformation(mDrawString);
    mHierarchy = new Hierarchy(mDrawString);
    mInspector = new Inspector(mDrawString);
}

void DebugUtility::loadProperties(const rapidjson::Value& inObj) {
    mDrawString->loadProperties(inObj);
    mLog->loadProperties(inObj);
    mFixedDebugInfo->loadProperties(inObj);
    mHierarchy->loadProperties(inObj);
    mInspector->loadProperties(inObj);
}

void DebugUtility::initialize(std::shared_ptr<Renderer> renderer) {
    mDrawString->initialize(renderer);
    mLog->initialize();
    mFixedDebugInfo->initialize();
    mHierarchy->initialize();
    mInspector->initialize();
}

void DebugUtility::finalize() {
    SAFE_DELETE(mInspector);
    SAFE_DELETE(mHierarchy);
    SAFE_DELETE(mFixedDebugInfo);
    SAFE_DELETE(mLog);
    SAFE_DELETE(mDrawString);
}

void DebugUtility::update() {
    mLog->update();
}

void DebugUtility::windowMessage(const std::string& message) {
#ifdef _DEBUG
    MessageBoxA(0, message.c_str(), 0, MB_OK);
#endif // _DEBUG
}

void DebugUtility::draw(const Matrix4& proj) {
#ifdef _DEBUG
    mLog->drawLogs(mDrawString);
    mFixedDebugInfo->draw();
    mHierarchy->drawActors();
    mInspector->drawInspect();
    mDrawString->drawAll(proj);
#endif // _DEBUG
}

void DebugUtility::drawStringClear() {
    mDrawString->clear();
}

Log* DebugUtility::log() {
    return mLog;
}

FixedDebugInformation* DebugUtility::fixedDebugInfo() {
    return mFixedDebugInfo;
}

Hierarchy* DebugUtility::hierarchy() {
    return mHierarchy;
}

Inspector* DebugUtility::inspector() {
    return mInspector;
}

DrawString* DebugUtility::mDrawString = nullptr;
Log* DebugUtility::mLog = nullptr;
FixedDebugInformation* DebugUtility::mFixedDebugInfo = nullptr;
Hierarchy* DebugUtility::mHierarchy = nullptr;
Inspector* DebugUtility::mInspector = nullptr;
