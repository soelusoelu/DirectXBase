#include "ResultRank.h"
#include "ComponentManager.h"
#include "SpriteComponent.h"
#include "../DebugLayer/Debug.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"

ResultRank::ResultRank() :
    mSprite(nullptr),
    mRank(Rank::NONE),
    mLow(0),
    mMiddle(0),
    mHigh(0),
    mLowTextureName(""),
    mMiddleTextureName(""),
    mHighTextureName(""),
    mSuperTextureName("") {
}

ResultRank::~ResultRank() = default;

void ResultRank::start() {
    mSprite = owner()->componentManager()->getComponent<SpriteComponent>();
    selectTexture();
    mSprite->setActive(false);
}

void ResultRank::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "low", &mLow);
    JsonHelper::getInt(inObj, "middle", &mMiddle);
    JsonHelper::getInt(inObj, "high", &mHigh);
    JsonHelper::getString(inObj, "lowTextureName", &mLowTextureName);
    JsonHelper::getString(inObj, "middleTextureName", &mMiddleTextureName);
    JsonHelper::getString(inObj, "highTextureName", &mHighTextureName);
    JsonHelper::getString(inObj, "superTextureName", &mSuperTextureName);
}

void ResultRank::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Low", mLow);
    inspect->emplace_back("Middle", mMiddle);
    inspect->emplace_back("High", mHigh);
    inspect->emplace_back("LowTextureName", mLowTextureName);
    inspect->emplace_back("MiddleTextureName", mMiddleTextureName);
    inspect->emplace_back("HighTextureName", mHighTextureName);
    inspect->emplace_back("SuperTextureName", mSuperTextureName);
}

void ResultRank::initialize(int score) {
    selectRank(score);
}

void ResultRank::setActive(bool value) {
    mSprite->setActive(value);
}

void ResultRank::selectRank(int score) {
    if (score < mLow) {
        mRank = Rank::LOW;
    } else if (mLow <= score && score < mMiddle) {
        mRank = Rank::MIDDLE;
    } else if (mMiddle <= score && score < mHigh) {
        mRank = Rank::HIGH;
    } else if (mHigh <= score) {
        mRank = Rank::SUPER;
    } else {
        Debug::log(getComponentName() + ": No rank");
    }
}

void ResultRank::selectTexture() {
    std::string fileName = "";
    if (mRank == Rank::LOW) {
        fileName = mLowTextureName;
    } else if (mRank == Rank::MIDDLE) {
        fileName = mMiddleTextureName;
    } else if (mRank == Rank::HIGH) {
        fileName = mHighTextureName;
    } else if (mRank == Rank::SUPER) {
        fileName = mSuperTextureName;
    } else {
        Debug::log(getComponentName() + ": Rank is None");
        return;
    }

    mSprite->setSprite(fileName);
}
