﻿#include "FriedChickenComponent.h"
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
    mRandomRangePositionX(Vector2::zero),
    mRandomRangePositionZ(Vector2::zero),
    mRandomRangeScale(Vector2::one),
    mInitColor(Vector3::zero),
    mFryedColor(Vector3::zero) {
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
}

void FriedChickenComponent::update() {
    frying();
    changeFryedColor();
}

void FriedChickenComponent::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "randomRangePositionX", &mRandomRangePositionX);
    JsonHelper::getVector2(inObj, "randomRangePositionZ", &mRandomRangePositionZ);
    JsonHelper::getVector2(inObj, "randomRangeScale", &mRandomRangeScale);
    JsonHelper::getVector3(inObj, "initColor", &mInitColor);
    JsonHelper::getVector3(inObj, "fryedColor", &mFryedColor);
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
    auto pos = Random::randomRange(
        Vector3(mRandomRangePositionX.x, 0.f, mRandomRangePositionZ.x),
        Vector3(mRandomRangePositionX.y, 0.f, mRandomRangePositionZ.y)
    );
    auto t = owner()->transform();
    t->setPosition(pos);
    auto scale = Random::randomRange(mRandomRangeScale.x, mRandomRangeScale.y);
    t->setScale(scale);

    mSurface = Surface::BOTTOM;
    for (size_t i = 0; i < static_cast<int>(Surface::NUM_SURFACE); i++) {
        mFryTimer[i]->reset();
    }
}

void FriedChickenComponent::changeSurface() {
    mSurface = (mSurface == Surface::UP) ? Surface::BOTTOM : Surface::UP;
}

bool FriedChickenComponent::successFrying() const {
    bool result = true;
    for (size_t i = 0; i < static_cast<int>(Surface::NUM_SURFACE); i++) {
        if (!mFryTimer[i]->isTime()) {
            result = false;
            break;
        }
    }

    return result;
}

void FriedChickenComponent::frying() {
    mFryTimer[static_cast<int>(mSurface)]->update();
}

void FriedChickenComponent::changeFryedColor() {
    auto upSurface = (mSurface == Surface::UP) ? Surface::BOTTOM : Surface::UP;
    auto rate = mFryTimer[static_cast<int>(upSurface)]->rate();
    if (rate > 1.f) {
        rate = 1.f;
    }
    auto upColor = Vector3::lerp(mInitColor, mFryedColor, rate);
    rate = mFryTimer[static_cast<int>(mSurface)]->rate();
    if (rate > 1.f) {
        rate = 1.f;
    }
    auto bottomColor = Vector3::lerp(mInitColor, mFryedColor, rate);
    if (mMeshComp) {
        mMeshComp->setUpColor(upColor);
        mMeshComp->setBottomColor(bottomColor);
    }
}
