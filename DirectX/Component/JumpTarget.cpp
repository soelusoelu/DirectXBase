#include "JumpTarget.h"
#include "ComponentManager.h"
#include "SpriteComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../Utility/LevelLoader.h"

JumpTarget::JumpTarget(std::shared_ptr<GameObject> owner) :
    Component(owner, "JumpTarget"),
    mSpriteComp(nullptr),
    mPosition(Vector2::zero),
    mTextureScale(Vector2::one) {
}

JumpTarget::~JumpTarget() = default;

void JumpTarget::start() {
    mSpriteComp = owner()->componentManager()->getComponent<SpriteComponent>();
    if (mSpriteComp) {
        mSpriteComp->transform()->setScale(mTextureScale);
    }
}

void JumpTarget::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "textureScale", &mTextureScale);
}

void JumpTarget::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setVector2(alloc, inObj, "textureScale", mTextureScale);
}

void JumpTarget::setTargetPosition(const Vector2& pos) {
    mPosition = pos;
}
