#include "FixedDebugInformation.h"
#include "DrawString.h"
#include "../Actor/Actor.h"
#include "../System/Game.h"

FixedDebugInformation::FixedDebugInformation(std::shared_ptr<DrawString> drawString) :
    mDrawString(drawString) {
}

FixedDebugInformation::~FixedDebugInformation() = default;

void FixedDebugInformation::drawFPS(float fps) const {
    auto drawPos = FPS_POS;
    std::string str = "fps";
    mDrawString->drawString(str, drawPos, SCALE);
    drawPos.x += DrawString::WIDTH * (str.length() + 1) * SCALE.x;
    mDrawString->drawNumber(fps, drawPos, SCALE, 2);
}

void FixedDebugInformation::drawActors(std::list<std::shared_ptr<Actor>> actors) {
    auto drawPos = ACTORS_POS;
    std::string str = "Actors";
    mDrawString->drawString(str, drawPos, SCALE);
    drawPos.x += DrawString::WIDTH * (str.length() + 1) * SCALE.x;

    for (const auto& actor : actors) {
        mDrawString->drawString(actor->tag(), drawPos, SCALE);
        drawPos.y += DrawString::HEIGHT * SCALE.y;
    }
}

const Vector2 FixedDebugInformation::SCALE = Vector2(0.3125f, 0.3125f);
const Vector2 FixedDebugInformation::FPS_POS = Vector2(Game::WINDOW_WIDTH / 2.f, Game::WINDOW_HEIGHT);
const Vector2 FixedDebugInformation::ACTORS_POS = FPS_POS + Vector2(0.f, DrawString::HEIGHT * SCALE.y);
const int FixedDebugInformation::NUM_ROWS_TO_DISPLAY = (Game::WINDOW_DEBUG_HEIGHT - Game::WINDOW_HEIGHT) / (DrawString::HEIGHT * SCALE.y) - 1;