#include "Score.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"

Score::Score(std::shared_ptr<GameObject> owner) :
    Component(owner, "Score"),
    mDrawPosition(Vector2::zero),
    mDrawScale(Vector2::one),
    mScore(0),
    mHighScore(0) {
}

Score::~Score() = default;

void Score::update() {
    if (mScore > mHighScore) {
        mHighScore = mScore;
    }
    auto ds = owner()->renderer()->getDrawString();
    ds->drawString("Score", mDrawPosition - Vector2(32.f * 14, 0.f) * mDrawScale, mDrawScale);
    ds->drawNumberRightJustified(mScore, mDrawPosition, mDrawScale);
    ds->drawString("HighScore", mDrawPosition + Vector2(-32.f * 14, 64.f) * mDrawScale, mDrawScale);
    ds->drawNumberRightJustified(mHighScore, mDrawPosition + Vector2(0.f, 64.f) * mDrawScale, mDrawScale);
}

void Score::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "position", &mDrawPosition);
    JsonHelper::getVector2(inObj, "scale", &mDrawScale);
    JsonHelper::getInt(inObj, "highScore", &mHighScore);
}

void Score::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setVector2(alloc, inObj, "position", mDrawPosition);
    JsonHelper::setVector2(alloc, inObj, "scale", mDrawScale);
    JsonHelper::setInt(alloc, inObj, "highScore", mHighScore);
}

void Score::addScore(int score) {
    mScore += score;
}
