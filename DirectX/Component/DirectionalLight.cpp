#include "DirectionalLight.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

DirectionalLight::DirectionalLight(std::shared_ptr<GameObject> owner) :
    Component(owner, "DirectionalLight"),
    mDirection(Vector3::zero),
    mColor(Vector3::one) {
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::onUpdateWorldTransform() {
    mDirection = Vector3::transform(Vector3::up, owner()->transform()->getRotation());
}

void DirectionalLight::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    //向きと色を設定
    JsonHelper::getVector3(inObj, "direction", &mDirection);
    JsonHelper::getVector3(inObj, "color", &mColor);
    owner()->transform()->rotate(mDirection);
}

void DirectionalLight::drawDebugInfo(DebugInfoList* inspect) const {
    DebugInfo info;
    info.first = "Color";
    info.second = StringUtil::vector3ToString(mColor);
    inspect->emplace_back(info);
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
