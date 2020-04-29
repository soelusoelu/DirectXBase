#include "JumpTarget.h"
#include "ComponentManager.h"
#include "Sprite3D.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

JumpTarget::JumpTarget(std::shared_ptr<GameObject> owner) :
    Component(owner, "JumpTarget"),
    mSprite(nullptr),
    mOffsetPosY(0.f) {
}

JumpTarget::~JumpTarget() = default;

void JumpTarget::start() {
    mSprite = owner()->componentManager()->getComponent<Sprite3D>();
}

void JumpTarget::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "offsetPosY", &mOffsetPosY);
}

void JumpTarget::saveProperties(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setFloat(alloc, inObj, "offsetPosY", mOffsetPosY);
}

void JumpTarget::setTargetPosition(const Vector3 & pos) {
    mSprite->transform()->setPosition(pos + Vector3::up * mOffsetPosY);
}
