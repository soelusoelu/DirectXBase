#include "FriedChickenComponent.h"
#include "ChickenFry.h"
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
    mFry(nullptr),
    mState(State::FRY),
    mRandomRangePositionX(Vector2::zero),
    mRandomRangePositionZ(Vector2::zero),
    mRandomRangeScale(Vector2::one),
    mGoodLevel(0.f),
    mRollSpeed(60.f),
    mFallSpeed(1.f) {
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
            friedChangeState();
        }
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
    JsonHelper::getFloat(inObj, "rollSpeed", &mRollSpeed);
    JsonHelper::getFloat(inObj, "fallSpeed", &mFallSpeed);
}

void FriedChickenComponent::drawDebugInfo(DebugInfoList * inspect) const {
    DebugInfo info;
    info.first = "RandomRangePositionX";
    info.second = InspectHelper::vector2ToString(mRandomRangePositionX);
    inspect->emplace_back(info);
    info.first = "RandomRangePositionZ";
    info.second = InspectHelper::vector2ToString(mRandomRangePositionZ);
    inspect->emplace_back(info);
    info.first = "RandomRangeScale";
    info.second = InspectHelper::vector2ToString(mRandomRangeScale);
    inspect->emplace_back(info);
    info.first = "RollSpeed";
    info.second = StringUtil::floatToString(mRollSpeed);
    inspect->emplace_back(info);
    info.first = "FallSpeed";
    info.second = StringUtil::floatToString(mFallSpeed);
    inspect->emplace_back(info);
}

void FriedChickenComponent::firstSet(float good) {
    mGoodLevel = good;
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
    if (mFry) {
        mFry->initialize();
    }

    //空中からの落下状態に設定
    mState = State::FALL;
}

void FriedChickenComponent::finishFryed() {
    mState = State::WAITING_COLLECTION;
}

void FriedChickenComponent::eaten() {
    mState = State::EATEN;
}

void FriedChickenComponent::roll(const Vector3& direction) {
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
    return mState == State::WAITING_COLLECTION;
}

bool FriedChickenComponent::isEaten() const {
    return mState == State::EATEN;
}

void FriedChickenComponent::friedChangeState() {
    if (mFry->isFriedAllSurfaces(mGoodLevel)) {
        mState = State::WAITING_COLLECTION;
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
