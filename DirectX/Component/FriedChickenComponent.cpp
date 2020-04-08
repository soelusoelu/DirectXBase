#include "FriedChickenComponent.h"
#include "ChickenMeshComponent.h"
#include "ComponentManager.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FriedChickenComponent::FriedChickenComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "FriedChickenComponent"),
    mMeshComp(nullptr),
    mSurface(Surface::BOTTOM),
    mState(State::FRY),
    mRandomRangePositionX(Vector2::zero),
    mRandomRangePositionZ(Vector2::zero),
    mRandomRangeScale(Vector2::one),
    mInitColor(Vector3::zero),
    mFryedColor(Vector3::zero),
    mBurntColor(Vector3::zero),
    mFallSpeed(60.f) {
    for (size_t i = 0; i < static_cast<int>(Surface::NUM_SURFACE); i++) {
        mFryTimer.emplace_back(std::make_unique<Time>(10.f));
    }
}

FriedChickenComponent::~FriedChickenComponent() = default;

void FriedChickenComponent::start() {
    mMeshComp = owner()->componentManager()->getComponent<ChickenMeshComponent>();
    if (mMeshComp) {
        mMeshComp->setColor(mInitColor);
    }

    initialize();
    //y軸を0にして最初から揚げ状態でスタート
    auto t = owner()->transform();
    auto pos = t->getPosition();
    pos.y = 0.f;
    t->setPosition(pos);
    mState = State::FRY;
}

void FriedChickenComponent::update() {
    if (mState == State::FRY) {
        frying();
        changeFryedColor();
        successFrying();
    } else if (mState == State::FALL) {
        fall();
        soakedInOil();
    }
}

void FriedChickenComponent::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "randomRangePositionX", &mRandomRangePositionX);
    JsonHelper::getVector2(inObj, "randomRangePositionZ", &mRandomRangePositionZ);
    JsonHelper::getVector2(inObj, "randomRangeScale", &mRandomRangeScale);
    JsonHelper::getVector3(inObj, "initColor", &mInitColor);
    JsonHelper::getVector3(inObj, "fryedColor", &mFryedColor);
    JsonHelper::getVector3(inObj, "burntColor", &mBurntColor);
    JsonHelper::getFloat(inObj, "fallSpeed", &mFallSpeed);
}

void FriedChickenComponent::drawDebugInfo(debugInfoList * inspect) const {
    debugInfo info;
    info.first = "RandomRangePositionX";
    info.second = InspectHelper::vector2ToString(mRandomRangePositionX);
    inspect->emplace_back(info);
    info.first = "RandomRangePositionZ";
    info.second = InspectHelper::vector2ToString(mRandomRangePositionZ);
    inspect->emplace_back(info);
    info.first = "RandomRangeScale";
    info.second = InspectHelper::vector2ToString(mRandomRangeScale);
    inspect->emplace_back(info);
    info.first = "State";
    if (mSurface == Surface::UP) {
        info.second = "FRONT";
    } else if (mSurface == Surface::BOTTOM) {
        info.second = "BACK";
    }
    inspect->emplace_back(info);
}

void FriedChickenComponent::initialize() {
    //アクティブ化
    owner()->setActive(true);

    //ランダムで位置を決める
    auto pos = Random::randomRange(
        Vector3(mRandomRangePositionX.x, 0.f, mRandomRangePositionZ.x),
        Vector3(mRandomRangePositionX.y, 0.f, mRandomRangePositionZ.y)
    );
    auto t = owner()->transform();
    t->setPosition(pos + Vector3::up * 10.f);
    //ランダムでスケールを決める
    auto scale = Random::randomRange(mRandomRangeScale.x, mRandomRangeScale.y);
    t->setScale(scale);

    //揚げる面の初期化
    mSurface = Surface::BOTTOM;
    for (size_t i = 0; i < static_cast<int>(Surface::NUM_SURFACE); i++) {
        mFryTimer[i]->reset();
    }
    if (mMeshComp) {
        mMeshComp->setUpColor(mInitColor);
        mMeshComp->setBottomColor(mInitColor);
    }

    //空中からの落下状態に設定
    mState = State::FALL;
}

void FriedChickenComponent::changeSurface() {
    mSurface = (mSurface == Surface::UP) ? Surface::BOTTOM : Surface::UP;
}

void FriedChickenComponent::finishFryed() {
    mState = State::WAITING_COLLECTION;
}

bool FriedChickenComponent::isFrying() const {
    return mState == State::FRY;
}

bool FriedChickenComponent::isFalling() const {
    return mState == State::FALL;
}

bool FriedChickenComponent::isFinished() const {
    return mState == State::WAITING_COLLECTION;
}

int FriedChickenComponent::evaluateScore() {
    int score = 0;

    for (size_t i = 0; i < static_cast<int>(Surface::NUM_SURFACE); i++) {

    }

    return score;
}

void FriedChickenComponent::frying() {
    mFryTimer[static_cast<int>(mSurface)]->update();
}

void FriedChickenComponent::changeFryedColor() {
    auto upSurface = (mSurface == Surface::UP) ? Surface::BOTTOM : Surface::UP;
    changeColor(upSurface);
    changeColor(mSurface);
}

void FriedChickenComponent::changeColor(Surface surface) {
    if (surface == Surface::NUM_SURFACE) {
        return;
    }

    //色の変更
    auto rate = mFryTimer[static_cast<int>(surface)]->rate();
    auto a = mInitColor;
    auto b = mFryedColor;
    if (rate > 1.f) {
        rate -= 1.f;
        a = mFryedColor;
        b = mBurntColor;
    }
    if (rate > 1.f) {
        rate = 1.f;
    }
    auto color = Vector3::lerp(a, b, rate);

    if (!mMeshComp) {
        return;
    }
    //適用する面
    if (surface == Surface::UP) {
        mMeshComp->setUpColor(color);
    } else if (surface == Surface::BOTTOM) {
        mMeshComp->setBottomColor(color);
    }
}

void FriedChickenComponent::successFrying() {
    bool result = true;
    for (size_t i = 0; i < static_cast<int>(Surface::NUM_SURFACE); i++) {
        if (!mFryTimer[i]->isTime()) {
            result = false;
            break;
        }
    }

    mState = (result) ? State::WAITING_COLLECTION : State::FRY;
}

void FriedChickenComponent::fall() {
    owner()->transform()->translate(Vector3::down * mFallSpeed * Time::deltaTime);
}

void FriedChickenComponent::soakedInOil() {
    auto t = owner()->transform();
    auto pos = t->getPosition();
    if (pos.y <= 0.f) {
        pos.y = 0.f;
        t->setPosition(pos);
        mState = State::FRY;
    }
}
