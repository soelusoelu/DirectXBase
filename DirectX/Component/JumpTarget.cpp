﻿#include "JumpTarget.h"
#include "ComponentManager.h"
#include "Sprite3D.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

JumpTarget::JumpTarget(std::shared_ptr<GameObject> owner) :
    Component(owner, "JumpTarget"),
    mSprite(nullptr),
    mOffsetPosY(0.f),
    mAmountRotation(0.f) {
}

JumpTarget::~JumpTarget() = default;

void JumpTarget::start() {
    mSprite = owner()->componentManager()->getComponent<Sprite3D>();
}

void JumpTarget::update() {
    mSprite->transform()->rotate(Vector3::up, mAmountRotation);
}

void JumpTarget::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "offsetPosY", &mOffsetPosY);
    JsonHelper::getFloat(inObj, "amountRotation", &mAmountRotation);
}

void JumpTarget::saveProperties(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setFloat(alloc, inObj, "offsetPosY", mOffsetPosY);
    JsonHelper::setFloat(alloc, inObj, "amountRotation", mAmountRotation);
}

void JumpTarget::setActive(bool value) {
    mSprite->setActive(value);
}

void JumpTarget::setTargetPosition(const Vector3 & pos) {
    mSprite->transform()->setPosition(pos + Vector3::up * mOffsetPosY);
}
