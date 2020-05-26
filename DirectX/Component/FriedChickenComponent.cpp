#include "FriedChickenComponent.h"
#include "ChickenFry.h"
#include "ChickenMeshComponent.h"
#include "ComponentManager.h"
#include "SoundComponent.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

FriedChickenComponent::FriedChickenComponent() :
    Component(),
    mFry(nullptr),
    mState(State::FRY),
    mRandomRangePositionX(Vector2::zero),
    mRandomRangePositionZ(Vector2::zero),
    mRandomRangeScale(Vector2::one),
    mRollSpeed(60.f),
    mFallSpeed(1.f),
    mIsWaitingColliction(false) {
}

FriedChickenComponent::~FriedChickenComponent() = default;

void FriedChickenComponent::start() {
    mFry = owner()->componentManager()->getComponent<ChickenFry>();

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
        if (mFry) {
            mFry->update();
            tooBurnt();
            autoCollection();
        }
    } else if (mState == State::FALL) {
        fall();
        soakedInOil();
    } else if (mState == State::TOO_BURNT) {
        tooBurntUpdate();
    }
}

void FriedChickenComponent::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getVector2(inObj, "randomRangePositionX", &mRandomRangePositionX);
    JsonHelper::getVector2(inObj, "randomRangePositionZ", &mRandomRangePositionZ);
    JsonHelper::getVector2(inObj, "randomRangeScale", &mRandomRangeScale);
    JsonHelper::getFloat(inObj, "rollSpeed", &mRollSpeed);
    JsonHelper::getFloat(inObj, "fallSpeed", &mFallSpeed);
}

void FriedChickenComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("RandomRangePositionX", mRandomRangePositionX);
    inspect->emplace_back("RandomRangePositionZ", mRandomRangePositionZ);
    inspect->emplace_back("RandomRangeScale", mRandomRangeScale);
    inspect->emplace_back("RollSpeed", mRollSpeed);
    inspect->emplace_back("FallSpeed", mFallSpeed);
}

const IChickenFry& FriedChickenComponent::getFry() const {
    return *mFry;
}

void FriedChickenComponent::initialize() {
    //アクティブ化
    owner()->setActive(true);

    mIsWaitingColliction = false;

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
    if (mFry) {
        mFry->initialize();
    }

    //空中からの落下状態に設定
    mState = State::FALL;
}

void FriedChickenComponent::finishFryed() {
    mIsWaitingColliction = true;
}

void FriedChickenComponent::eaten() {
    mState = State::EATEN;
}

void FriedChickenComponent::roll(const Vector3 & direction) {
    auto dir = direction;
    dir.x = direction.z;
    dir.z = -direction.x;
    owner()->transform()->rotate(dir * Time::deltaTime * mRollSpeed);
}

bool FriedChickenComponent::isFrying() const {
    return mState == State::FRY;
}

bool FriedChickenComponent::isFalling() const {
    return mState == State::FALL;
}

bool FriedChickenComponent::isFinished() const {
    return mIsWaitingColliction;
}

bool FriedChickenComponent::isEaten() const {
    return mState == State::EATEN;
}

bool FriedChickenComponent::isTooBurnt() const {
    return mState == State::TOO_BURNT;
}

void FriedChickenComponent::tooBurnt() {
    if (mFry->isTooBurnt()) {
        auto sound = owner()->componentManager()->getComponent<SoundComponent>();
        sound->playSE();

        mState = State::TOO_BURNT;
    }
}

void FriedChickenComponent::tooBurntUpdate() {
    auto trans = owner()->transform();
    trans->translate(Vector3::down * Time::deltaTime);

    if (trans->getPosition().y < -3.f) {
        finishFryed();
    }
}

void FriedChickenComponent::autoCollection() {
    if (mFry->isBurntAllSurfaces()) {
        finishFryed();
    }
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
