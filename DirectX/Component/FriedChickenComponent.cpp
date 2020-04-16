#include "FriedChickenComponent.h"
#include "ChickenMeshComponent.h"
#include "ComponentManager.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/Material.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FriedChickenComponent::FriedChickenComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "FriedChickenComponent"),
    mState(State::FRY),
    mRandomRangePositionX(Vector2::zero),
    mRandomRangePositionZ(Vector2::zero),
    mRandomRangeScale(Vector2::one),
    mInitColor(Vector3::zero),
    mFryedColor(Vector3::zero),
    mBurntColor(Vector3::zero),
    mUpColor(nullptr),
    mBottomColor(nullptr),
    mLeftColor(nullptr),
    mRightColor(nullptr),
    mForeColor(nullptr),
    mBackColor(nullptr),
    mGood(0.f),
    mFallSpeed(60.f) {
    for (size_t i = 0; i < getNumSurface(); i++) {
        mFryTimer.emplace_back(std::make_unique<Time>(10.f));
    }
}

FriedChickenComponent::~FriedChickenComponent() = default;

void FriedChickenComponent::start() {
    auto mesh = owner()->componentManager()->getComponent<ChickenMeshComponent>();
    if (mesh) {
        Debug::log(StringUtil::intToString(mesh->getNumMaterial()));
        mRightColor = mesh->getMaterial(1); //右
        mForeColor = mesh->getMaterial(2); //手前
        mLeftColor = mesh->getMaterial(3); //左
        mUpColor = mesh->getMaterial(4); //上
        mBottomColor = mesh->getMaterial(5); //下
        mBackColor = mesh->getMaterial(6); //奥
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
}

void FriedChickenComponent::firstSet(float good) {
    mGood = good;
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
    for (size_t i = 0; i < getNumSurface(); i++) {
        mFryTimer[i]->reset();
    }
    mUpColor->diffuse = Vector4(mInitColor, 1.f);
    mBottomColor->diffuse = Vector4(mInitColor, 1.f);
    mLeftColor->diffuse = Vector4(mInitColor, 1.f);
    mRightColor->diffuse = Vector4(mInitColor, 1.f);
    mForeColor->diffuse = Vector4(mInitColor, 1.f);
    mBackColor->diffuse = Vector4(mInitColor, 1.f);

    //空中からの落下状態に設定
    mState = State::FALL;
}

void FriedChickenComponent::changeSurface() {
}

void FriedChickenComponent::finishFryed() {
    mState = State::WAITING_COLLECTION;
}

void FriedChickenComponent::eaten() {
    mState = State::EATEN;
}

int FriedChickenComponent::getNumSurface() const {
    return static_cast<int>(Surface::NUM_SURFACE);
}

float FriedChickenComponent::getFryRate(int surfaceIndex) const {
    return mFryTimer[surfaceIndex]->rate();
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

void FriedChickenComponent::frying() {
    mFryTimer[static_cast<int>(Surface::BOTTOM)]->update();
}

void FriedChickenComponent::changeFryedColor() {
    auto color = getChangeColor(Surface::BOTTOM);
    mForeColor->diffuse = Vector4(color, 1.f);
}

Vector3 FriedChickenComponent::getChangeColor(Surface surface) {
    if (surface == Surface::NUM_SURFACE) {
        return Vector3::zero;
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
    return Vector3::lerp(a, b, rate);
}

void FriedChickenComponent::successFrying() {
    bool result = true;
    for (size_t i = 0; i < getNumSurface(); i++) {
        if (mFryTimer[i]->rate() < mGood) {
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
