#include "Log.h"
#include "DrawString.h"
#include "Renderer.h"
#include "../System/Game.h"

Log::Log(std::shared_ptr<Renderer> renderer) :
    mDrawString(renderer->getDrawString()) {
    mLogs.resize(NUM_LOG);
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
    auto pos = Vector2(0.f, Game::WINDOW_DEBUG_HEIGHT - 64.f * NUM_LOG * SCALE.y);
    for (const auto& log : mLogs) {
        mDrawString->drawString(log.first, pos, SCALE, log.second);
        pos.y += 64.f * SCALE.y;
    }
#endif // _DEBUG
}

void Log::adjustCapacity() {
    while (mLogs.size() > NUM_LOG) {
        mLogs.pop_front();
    }
}

const Vector2 Log::SCALE = Vector2(0.35f, 0.35f);
