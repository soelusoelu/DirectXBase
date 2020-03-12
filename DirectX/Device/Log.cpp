#include "Log.h"
#include "DrawString.h"
#include "../System/Game.h"

Log::Log(std::shared_ptr<DrawString> drawString) :
    mDrawString(drawString) {
    mLogs.resize(NUM_ROWS_TO_DISPLAY);
}

Log::~Log() = default;

void Log::log(const std::string& message) {
    mLogs.emplace_back(std::make_pair(message, ColorPalette::white));
}

void Log::logError(const std::string& message) {
    mLogs.emplace_back(std::make_pair(message, ColorPalette::red));
}

void Log::logWarning(const std::string& message) {
    mLogs.emplace_back(std::make_pair(message, ColorPalette::yellow));
}

void Log::drawLogs() {
    adjustCapacity();

#ifdef _DEBUG
    auto pos = Vector2(0.f, Game::WINDOW_DEBUG_HEIGHT - DrawString::HEIGHT * NUM_ROWS_TO_DISPLAY * SCALE.y);
    for (const auto& log : mLogs) {
        mDrawString->drawString(log.first, pos, SCALE, log.second);
        pos.y += DrawString::HEIGHT * SCALE.y;
    }
#endif // _DEBUG
}

void Log::adjustCapacity() {
    while (mLogs.size() > NUM_ROWS_TO_DISPLAY) {
        mLogs.pop_front();
    }
}

const Vector2 Log::SCALE = Vector2(0.3125f, 0.3125f);
const int Log::NUM_ROWS_TO_DISPLAY = (Game::WINDOW_DEBUG_HEIGHT - Game::WINDOW_HEIGHT) / (DrawString::HEIGHT * SCALE.y);
