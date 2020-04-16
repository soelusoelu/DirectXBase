﻿#include "FriedChickenComponent.h"
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
    mCurrentBottomSurface(Surface::BOTTOM),
    mGood(0.f),
    mFallSpeed(1.f) {
    for (size_t i = 0; i < getNumSurface(); i++) {
        mFryTimer.emplace_back(std::make_unique<Time>(10.f));
    }
}

FriedChickenComponent::~FriedChickenComponent() = default;

void FriedChickenComponent::start() {
    auto mesh = owner()->componentManager()->getComponent<ChickenMeshComponent>();
    if (mesh) {
        for (size_t i = 0; i < getNumSurface(); i++) {
            //Surfaceと合わせてある
            //最初のマテリアルは使われてないからプラス1
            mMaterials.emplace_back(mesh->getMaterial(i + 1));
        }
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
        bottomSurface();
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
    for (auto&& timer : mFryTimer) {
        timer->reset();
    }
    for (auto&& mat : mMaterials) {
        mat->diffuse = mInitColor;
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

void FriedChickenComponent::bottomSurface() {
    auto dir = Vector3::transform(Vector3::down, owner()->transform()->getRotation());
    auto x = Math::cos(dir.x);
    auto y = Math::sin(dir.y);
    static const float SIN_COS_45 = Math::sin(45.f);
    if (dir.y <= -SIN_COS_45) {
        mCurrentBottomSurface = Surface::BOTTOM;
    } else if (dir.y > SIN_COS_45) {
        mCurrentBottomSurface = Surface::UP;
    } else if (dir.x <= -SIN_COS_45) {
        mCurrentBottomSurface = Surface::LEFT;
    } else if (dir.x > SIN_COS_45) {
        mCurrentBottomSurface = Surface::RIGHT;
    } else if (dir.z <= SIN_COS_45) {
        mCurrentBottomSurface = Surface::FORE;
    } else if (dir.z > SIN_COS_45) {
        mCurrentBottomSurface = Surface::BACK;
    } else {
        Debug::logError("No bottom Surface!");
    }
}

void FriedChickenComponent::frying() {
    mFryTimer[static_cast<int>(mCurrentBottomSurface)]->update();
}

void FriedChickenComponent::changeFryedColor() {
    auto rate = mFryTimer[static_cast<int>(mCurrentBottomSurface)]->rate();
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
    auto mat = getMaterial(mCurrentBottomSurface);
    mat->diffuse = color;
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

std::shared_ptr<Material> FriedChickenComponent::getMaterial(Surface surface) const {
    int index = static_cast<int>(surface);
    return mMaterials[index];
}
