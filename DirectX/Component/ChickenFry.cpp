﻿#include "ChickenFry.h"
#include "ChickenColorChanger.h"
#include "ChickenExclamation.h"
#include "ComponentManager.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

ChickenFry::ChickenFry() :
    Component(),
    mColorChanger(nullptr),
    mExclamation(nullptr),
    mCurrentBottomSurface(ChickenSurface::BOTTOM),
    mEasySurface(ChickenSurface::BOTTOM),
    mHardSurface(ChickenSurface::BOTTOM),
    mTooBurntTimer(std::make_unique<Time>()),
    mUsually(getNumFryState() - 1),
    mEasy(getNumFryState() - 1),
    mHard(getNumFryState() - 1),
    mBurntTime(0.f),
    mEasyBurntTime(0.f),
    mHardBurntTime(0.f) {
}

ChickenFry::~ChickenFry() = default;

void ChickenFry::awake() {
    for (size_t i = 0; i < getNumFryState() - 1; i++) {
        mBurntTime += mUsually[i];
        mEasyBurntTime += mEasy[i];
        mHardBurntTime += mHard[i];
    }

    for (size_t i = 0; i < getNumSurface(); i++) {
        mFryTimer.emplace_back(std::make_shared<Time>(mBurntTime));
    }
}

void ChickenFry::start() {
    mColorChanger = owner()->componentManager()->getComponent<ChickenColorChanger>();
    mExclamation = owner()->componentManager()->getComponent<ChickenExclamation>();
}

void ChickenFry::onUpdateWorldTransform() {
    auto previous = mCurrentBottomSurface;
    choiceBottomSurface();
    if (previous != mCurrentBottomSurface) {
        resetBurnt();
        mTooBurntTimer->reset();
    }
}

void ChickenFry::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "notFriedToLittleBadTimer", &mUsually[0]);
    JsonHelper::getFloat(inObj, "littleBadToUsuallyTimer", &mUsually[1]);
    JsonHelper::getFloat(inObj, "usuallyToGoodTimer", &mUsually[2]);
    JsonHelper::getFloat(inObj, "goodToBadTimer", &mUsually[3]);

    JsonHelper::getFloat(inObj, "easyNotFriedToLittleBadTimer", &mEasy[0]);
    JsonHelper::getFloat(inObj, "easyLittleBadToUsuallyTimer", &mEasy[1]);
    JsonHelper::getFloat(inObj, "easyUsuallyToGoodTimer", &mEasy[2]);
    JsonHelper::getFloat(inObj, "easyGoodToBadTimer", &mEasy[3]);

    JsonHelper::getFloat(inObj, "hardNotFriedToLittleBadTimer", &mHard[0]);
    JsonHelper::getFloat(inObj, "hardLittleBadToUsuallyTimer", &mHard[1]);
    JsonHelper::getFloat(inObj, "hardUsuallyToGoodTimer", &mHard[2]);
    JsonHelper::getFloat(inObj, "hardGoodToBadTimer", &mHard[3]);

    float time;
    if (JsonHelper::getFloat(inObj, "tooBurntTimer", &time)) {
        mTooBurntTimer->setLimitTime(time);
    }
}

void ChickenFry::drawDebugInfo(ComponentDebug::DebugInfoList * inspect) const {
    inspect->emplace_back("CurrentBottomSurface", surfaceToString(mCurrentBottomSurface));
    inspect->emplace_back("EasySurface", surfaceToString(mEasySurface));
    inspect->emplace_back("HardSurface", surfaceToString(mHardSurface));
    inspect->emplace_back("TooBurntTiemr", mTooBurntTimer->countUpTime());
}

void ChickenFry::initialize() {
    //揚げる面の初期化
    for (auto&& timer : mFryTimer) {
        timer->setLimitTime(mBurntTime);
        timer->reset();
    }
    //焦げ時間の初期化
    mTooBurntTimer->reset();

    choiceEasyAndHardSurface();

    if (mColorChanger) {
        mColorChanger->initialize();
    }
    if (mExclamation) {
        mExclamation->setActive(false);
    }
}

void ChickenFry::fryUpdate() {
    frying();
    updateTimerIfBurntBottomSurface();

    mColorChanger->update(mCurrentBottomSurface, getFryState(mCurrentBottomSurface));
    if (isTooBurnt()) {
        mColorChanger->changeAllSurfaces(FryState::BAD);
    }
    drawExclamation();
}

bool ChickenFry::isBurntAllSurfaces() const {
    bool result = true;
    for (size_t i = 0; i < getNumSurface(); i++) {
        if (getFryState(i) != FryState::BAD) {
            result = false;
            break;
        }
    }

    return result;
}

bool ChickenFry::isTooBurnt() const {
    return mTooBurntTimer->isTime();
}

bool ChickenFry::isBurntHalfSurfaces() const {
    int count = 0;
    for (size_t i = 0; i < getNumSurface(); i++) {
        auto state = getFryState(i);
        if (state == FryState::GOOD || state == FryState::BAD) {
            count++;
        }
    }

    return (count >= getNumSurface() / 2);
}

bool ChickenFry::isUpSelectState(FryState state) const {
    for (size_t i = 0; i < getNumSurface(); i++) {
        auto s = getFryState(i);
        if (static_cast<unsigned>(s) < static_cast<unsigned>(state)) {
            return false;
        }
    }

    return true;
}

FryState ChickenFry::getFryState(ChickenSurface surface) const {
    return getFryState(static_cast<unsigned>(surface));
}

FryState ChickenFry::getFryState(unsigned surfaceIndex) const {
    std::vector<float> timer;
    if (surfaceIndex == static_cast<unsigned>(mEasySurface)) {
        timer = mEasy;
    } else if (surfaceIndex == static_cast<unsigned>(mHardSurface)) {
        timer = mHard;
    } else {
        timer = mUsually;
    }

    FryState state = FryState::NOT_FRIED;
    auto current = mFryTimer[surfaceIndex]->countUpTime();
    float sum = 0.f;
    for (size_t i = 0; i < getNumFryState() - 1; i++) {
        sum += timer[i];
        if (current < sum) {
            state = static_cast<FryState>(i);
            break;
        }
    }
    if (current > sum) {
        state = FryState::BAD;
    }

    return state;
}

void ChickenFry::choiceEasyAndHardSurface() {
    auto easySurface = Random::randomRange(0, getNumSurface());
    int hardSurface;
    do {
        hardSurface = Random::randomRange(0, getNumSurface());
    } while (hardSurface == easySurface);

    mEasySurface = static_cast<ChickenSurface>(easySurface);
    mHardSurface = static_cast<ChickenSurface>(hardSurface);
    mFryTimer[static_cast<int>(mEasySurface)]->setLimitTime(mEasyBurntTime);
    mFryTimer[static_cast<int>(mHardSurface)]->setLimitTime(mHardBurntTime);
}

void ChickenFry::choiceBottomSurface() {
    static const Vector3 dirs[] = {
        Vector3::up,
        Vector3::down,
        Vector3::left,
        Vector3::right,
        Vector3::forward,
        Vector3::back,
    };
    static const ChickenSurface dirSurfaces[] = {
        ChickenSurface::UP,
        ChickenSurface::BOTTOM,
        ChickenSurface::RIGHT,
        ChickenSurface::LEFT,
        ChickenSurface::FORE,
        ChickenSurface::BACK,
    };

    float min = Math::infinity;
    auto sur = ChickenSurface::UP;
    for (size_t i = 0; i < 6; i++) {
        auto dir = Vector3::transform(dirs[i], owner()->transform()->getRotation());
        if (dir.y < min) {
            min = dir.y;
            sur = dirSurfaces[i];
        }
    }
    mCurrentBottomSurface = sur;
}

void ChickenFry::frying() {
    mFryTimer[static_cast<int>(mCurrentBottomSurface)]->update();
}

void ChickenFry::updateTimerIfBurntBottomSurface() {
    if (getFryState(mCurrentBottomSurface) != FryState::BAD) {
        return;
    }

    mTooBurntTimer->update();
}

void ChickenFry::resetBurnt() {
    for (size_t i = 0; i < 6; i++) {
        if (getFryState(i) != FryState::GOOD) {
            continue;
        }
        auto sumUsually = mBurntTime - mUsually[3];
        auto sumEasy = mEasyBurntTime - mEasy[3];
        auto sumHard = mHardBurntTime - mHard[3];
        if (i == static_cast<unsigned>(mEasySurface)) {
            mFryTimer[i]->setCurrentTime(sumEasy);
        } else if (i == static_cast<unsigned>(mHardSurface)) {
            mFryTimer[i]->setCurrentTime(sumHard);
        } else {
            mFryTimer[i]->setCurrentTime(sumUsually);
        }
    }
}

int ChickenFry::getNumSurface() const {
    return static_cast<int>(ChickenSurface::NUM_SURFACE);
}

int ChickenFry::getNumFryState() const {
    return static_cast<int>(FryState::NUM_STATE);
}

std::string ChickenFry::surfaceToString(ChickenSurface surface) const {
    std::string str;

    if (surface == ChickenSurface::UP) {
        str = "UP";
    } else if (surface == ChickenSurface::BOTTOM) {
        str = "BOTTOM";
    } else if (surface == ChickenSurface::LEFT) {
        str = "LEFT";
    } else if (surface == ChickenSurface::RIGHT) {
        str = "RIGHT";
    } else if (surface == ChickenSurface::FORE) {
        str = "FORE";
    } else if (surface == ChickenSurface::BACK) {
        str = "BACK";
    }

    return str;
}

void ChickenFry::drawExclamation() {
    if (getFryState(mCurrentBottomSurface) != FryState::GOOD) {
        mExclamation->setActive(false);
        return;
    }

    float burntTime = 0.f;
    auto bottom = static_cast<unsigned>(mCurrentBottomSurface);
    if (bottom == static_cast<unsigned>(mEasySurface)) {
        burntTime = mEasyBurntTime;
    } else if (bottom == static_cast<unsigned>(mHardSurface)) {
        burntTime = mHardBurntTime;
    } else {
        burntTime = mBurntTime;
    }

    if (mFryTimer[bottom]->countDownTime() < mExclamation->getTime()) {
        mExclamation->setActive(true);
        mExclamation->setPosition(owner()->transform()->getPosition());
    }
}
