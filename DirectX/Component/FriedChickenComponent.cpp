#include "FriedChickenComponent.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FriedChickenComponent::FriedChickenComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "FriedChickenComponent"),
    mState(State::FRONT),
    mRandomRangePositionX(Vector2::zero),
    mRandomRangePositionZ(Vector2::zero),
    mRandomRangeScale(Vector2::one) {
    for (size_t i = 0; i < static_cast<int>(State::NUM_SURFACE); i++) {
        mFryTimer.emplace_back(std::make_unique<Time>(2.f));
    }
}

FriedChickenComponent::~FriedChickenComponent() = default;

void FriedChickenComponent::start() {
    initialize();
}

void FriedChickenComponent::update() {
    frying();
}

void FriedChickenComponent::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "randomRangePositionX", &mRandomRangePositionX);
    JsonHelper::getVector2(inObj, "randomRangePositionZ", &mRandomRangePositionZ);
    JsonHelper::getVector2(inObj, "randomRangeScale", &mRandomRangeScale);
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
    if (mState == State::FRONT) {
        info.second = "FRONT";
    } else if (mState == State::BACK) {
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

    mState = State::FRONT;
    for (size_t i = 0; i < static_cast<int>(State::NUM_SURFACE); i++) {
        mFryTimer[i]->reset();
    }
}

void FriedChickenComponent::changeSurface() {
    mState = (mState == State::FRONT) ? State::BACK : State::FRONT;
}

bool FriedChickenComponent::successFrying() const {
    bool result = true;
    for (size_t i = 0; i < static_cast<int>(State::NUM_SURFACE); i++) {
        if (!mFryTimer[i]->isTime()) {
            result = false;
            break;
        }
    }

    return result;
}

void FriedChickenComponent::frying() {
    mFryTimer[static_cast<int>(mState)]->update();
}
