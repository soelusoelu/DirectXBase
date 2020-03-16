#include "Log.h"
#include "DrawString.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Log::Log() :
    mScale(Vector2::one),
    mNumRowsToDisplay(0) {
}

Log::~Log() = default;

void Log::loadProperties(const rapidjson::Value & inObj) {
    const auto& logObj = inObj["log"];
    if (logObj.IsObject()) {
        JsonHelper::getVector2(logObj, "scale", &mScale);
    }
}

void Log::initialize() {
    mNumRowsToDisplay = (Window::debugHeight() - Window::height()) / (DrawString::HEIGHT * mScale.y);
    mLogs.resize(mNumRowsToDisplay);
}

void Log::log(const std::string & message) {
    mLogs.emplace_back(std::make_pair(message, ColorPalette::white));
}

void Log::logError(const std::string & message) {
    mLogs.emplace_back(std::make_pair(message, ColorPalette::red));
}

void Log::logWarning(const std::string & message) {
    mLogs.emplace_back(std::make_pair(message, ColorPalette::yellow));
}

void Log::drawLogs(DrawString* drawString) {
    adjustCapacity();

#ifdef _DEBUG
    auto pos = Vector2(0.f, Window::debugHeight() - DrawString::HEIGHT * mNumRowsToDisplay * mScale.y);
    for (const auto& log : mLogs) {
        drawString->drawString(log.first, pos, mScale, log.second);
        pos.y += DrawString::HEIGHT * mScale.y;
    }
#endif // _DEBUG
}

void Log::adjustCapacity() {
    while (mLogs.size() > mNumRowsToDisplay) {
        mLogs.pop_front();
    }
}
