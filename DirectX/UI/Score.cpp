#include "Score.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../Utility/Input.h"
#include "../Utility/Keyboard.h"
#include "../Utility/LevelLoader.h"

Score::Score(std::shared_ptr<Renderer> renderer) :
    UI(renderer, "Score"),
    mDrawPosition(Vector2::zero),
    mDrawScale(Vector2::one),
    mScore(0),
    mHighScore(0) {
}

Score::~Score() = default;

void Score::updateUI() {
    if (Input::keyboard()->getKeyDown(KeyCode::U)) {
        mScore += 10;
    }
    if (mScore > mHighScore) {
        mHighScore = mScore;
    }
    auto ds = mRenderer->getDrawString();
    ds->drawString("Score", mDrawPosition - Vector2(32.f * 14, 0.f) * mDrawScale, mDrawScale);
    ds->drawNumberRightJustified(mScore, mDrawPosition, mDrawScale);
    ds->drawString("HighScore", mDrawPosition + Vector2(-32.f * 14, 64.f) * mDrawScale, mDrawScale);
    ds->drawNumberRightJustified(mHighScore, mDrawPosition + Vector2(0.f, 64.f) * mDrawScale, mDrawScale);
}

void Score::loadProperties(const rapidjson::Value& inObj) {
    UI::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "position", &mDrawPosition);
    JsonHelper::getVector2(inObj, "scale", &mDrawScale);
    JsonHelper::getInt(inObj, "highScore", &mHighScore);
}

void Score::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    UI::saveProperties(alloc, inObj);

    JsonHelper::setVector2(alloc, inObj, "position", mDrawPosition);
    JsonHelper::setVector2(alloc, inObj, "scale", mDrawScale);
    JsonHelper::setInt(alloc, inObj, "highScore", mHighScore);
}
