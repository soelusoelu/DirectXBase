#include "FriedChickenComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Utility/StringUtil.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Random.h"
#include "../Utility/LevelLoader.h"

FriedChickenComponent::FriedChickenComponent(std::shared_ptr<Actor> owner) :
    Component(owner, "FriedChickenComponent"),
    mRandomRangePositionX(Vector2::zero),
    mRandomRangePositionZ(Vector2::zero),
    mRandomRangeScale(Vector2::one) {
}

FriedChickenComponent::~FriedChickenComponent() = default;

void FriedChickenComponent::start() {
    initialize();
}

void FriedChickenComponent::update() {
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
}
