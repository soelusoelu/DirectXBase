#include "DirectionalLight.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

DirectionalLight::DirectionalLight() :
    Component(),
    mDirection(Vector3::zero),
    mColor(Vector3::one) {
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::awake() {
    owner()->transform()->rotate(mDirection);
}

void DirectionalLight::onUpdateWorldTransform() {
    mDirection = Vector3::transform(Vector3::up, owner()->transform()->getRotation());
}

void DirectionalLight::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector3(inObj, "direction", &mDirection);
    JsonHelper::getVector3(inObj, "color", &mColor);
}

void DirectionalLight::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Color", mColor);
}

const Vector3& DirectionalLight::getDirection() const {
    return mDirection;
}

void DirectionalLight::setDirection(const Vector3& dir) {
    mDirection = dir;
}

const Vector3& DirectionalLight::getColor() const {
    return mColor;
}

void DirectionalLight::setColor(const Vector3& color) {
    mColor = color;
}
