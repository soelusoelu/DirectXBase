#include "ChickenFry.h"
#include "ChickenColorChanger.h"
#include "ComponentManager.h"
#include "../DebugLayer/Debug.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

ChickenFry::ChickenFry(std::shared_ptr<GameObject> owner) :
    Component(owner, "ChickenFry"),
    mColorChanger(nullptr),
    mCurrentBottomSurface(ChickenSurface::BOTTOM),
    mEasySurface(ChickenSurface::BOTTOM),
    mHardSurface(ChickenSurface::BOTTOM),
    mTooBurntTimer(std::make_unique<Time>(0.f)),
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
}

void ChickenFry::onUpdateWorldTransform() {
    auto previous = mCurrentBottomSurface;
    choiceBottomSurface();
    if (previous != mCurrentBottomSurface) {
        mTooBurntTimer->reset();
    }
}

void ChickenFry::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

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

void ChickenFry::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "CurrentBottomSurface";
    info.second = surfaceToString(mCurrentBottomSurface);
    inspect->emplace_back(info);

    info.first = "EasySurface";
    info.second = surfaceToString(mEasySurface);
    inspect->emplace_back(info);

    info.first = "HardSurface";
    info.second = surfaceToString(mHardSurface);
    inspect->emplace_back(info);

    info.first = "TooBurntTiemr";
    info.second = StringUtil::floatToString(mTooBurntTimer->currentTime());
    inspect->emplace_back(info);

    info.first = "Euler";
    auto euler = owner()->transform()->getRotation().euler();
    info.second = StringUtil::vector3ToString(euler);
    inspect->emplace_back(info);
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
}

void ChickenFry::update() {
    frying();
    updateTimerIfBurntBottomSurface();

    if (mColorChanger) {
        mColorChanger->update(mCurrentBottomSurface, getFryState(mCurrentBottomSurface));
    }
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
    auto current = mFryTimer[surfaceIndex]->currentTime();
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
    auto easySurface = Random::randomRange(0, getNumSurface() - 1);
    int hardSurface;
    do {
        hardSurface = Random::randomRange(0, getNumSurface() - 1);
    } while (hardSurface == easySurface);

    mEasySurface = static_cast<ChickenSurface>(easySurface);
    mHardSurface = static_cast<ChickenSurface>(hardSurface);
    mFryTimer[static_cast<int>(mEasySurface)]->setLimitTime(mEasyBurntTime);
    mFryTimer[static_cast<int>(mHardSurface)]->setLimitTime(mHardBurntTime);
}

void ChickenFry::choiceBottomSurface() {
    auto dir = Vector3::transform(Vector3::down, owner()->transform()->getRotation());
    static const float SIN_COS_45 = Math::sin(45.f);
    if (dir.y <= -SIN_COS_45) {
        mCurrentBottomSurface = ChickenSurface::BOTTOM;
    } else if (dir.y >= SIN_COS_45) {
        mCurrentBottomSurface = ChickenSurface::UP;
    } else if (dir.x <= -SIN_COS_45) {
        mCurrentBottomSurface = ChickenSurface::RIGHT;
    } else if (dir.x >= SIN_COS_45) {
        mCurrentBottomSurface = ChickenSurface::LEFT;
    } else if (dir.z <= -SIN_COS_45) {
        mCurrentBottomSurface = ChickenSurface::BACK;
    } else if (dir.z >= SIN_COS_45) {
        mCurrentBottomSurface = ChickenSurface::FORE;
    } else {
        Debug::logError("No bottom Surface!");
    }
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
