#include "DebugUtility.h"
#include "FixedDebugInformation.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "Log.h"
#include "Pause.h"
#include "../Device/DrawString.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"

void DebugUtility::create() {
    mDrawString = new DrawString();
    mLog = new Log();
    mFixedDebugInfo = new FixedDebugInformation(mDrawString);
    mHierarchy = new Hierarchy(mDrawString);
    mInspector = new Inspector(mDrawString);
    mPause = new Pause();
}

void DebugUtility::loadProperties(const rapidjson::Value& inObj) {
    mDrawString->loadProperties(inObj);
    mLog->loadProperties(inObj);
    mFixedDebugInfo->loadProperties(inObj);
    mHierarchy->loadProperties(inObj);
    mInspector->loadProperties(inObj);
    mPause->loadProperties(inObj);
}

void DebugUtility::initialize() {
    mDrawString->initialize();
    mLog->initialize();
    mFixedDebugInfo->initialize();
    mHierarchy->initialize();
    mInspector->initialize();
    mPause->initialize();
}

void DebugUtility::finalize() {
    SAFE_DELETE(mPause);
    SAFE_DELETE(mInspector);
    SAFE_DELETE(mHierarchy);
    SAFE_DELETE(mFixedDebugInfo);
    SAFE_DELETE(mLog);
    SAFE_DELETE(mDrawString);
}

void DebugUtility::update() {
    mHierarchy->update();
    mLog->update();
    mPause->update();
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
    mPause->drawButton(proj);
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

Pause* DebugUtility::pause() {
    return mPause;
}
