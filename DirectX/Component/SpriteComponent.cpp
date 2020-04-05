#include "SpriteComponent.h"
#include "../GameObject/GameObject.h"
#include "../Sprite/Sprite.h"
#include "../Utility/LevelLoader.h"

SpriteComponent::SpriteComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "SpriteComponent"),
    mSprite(nullptr) {
}

SpriteComponent::~SpriteComponent() {
    mSprite->destroy();
}

void SpriteComponent::onSetActive(bool value) {
    setActive(value);
}

void SpriteComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string fileName;
    if (JsonHelper::getString(inObj, "fileName", &fileName)) {
        setSprite(fileName);
    }
}

void SpriteComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setString(alloc, inObj, "fileName", mSprite->fileName());
}

void SpriteComponent::setSprite(const std::string& fileName) {
    mSprite = std::make_shared<Sprite>(fileName);
    mSprite->addToManager();
}

std::shared_ptr<Transform2D> SpriteComponent::transform() const {
    return mSprite->transform();
}

void SpriteComponent::setColor(const Vector3& color) {
    mSprite->setColor(color);
}

void SpriteComponent::setColor(float r, float g, float b) {
    mSprite->setColor(r, g, b);
}

void SpriteComponent::setAlpha(float alpha) {
    mSprite->setAlpha(alpha);
}

const Vector4& SpriteComponent::getColor() const {
    return mSprite->getColor();
}

void SpriteComponent::setUV(float l, float t, float r, float b) {
    mSprite->setUV(l, t, r, b);
}

const Vector4& SpriteComponent::getUV() const {
    return mSprite->getUV();
}

const Vector2& SpriteComponent::getTextureSize() const {
    return mSprite->getTextureSize();
}

void SpriteComponent::setActive(bool value) {
    mSprite->setActive(value);
}

bool SpriteComponent::getActive() const {
    return mSprite->getActive();
}

std::shared_ptr<Texture> SpriteComponent::texture() const {
    return mSprite->texture();
}

std::shared_ptr<Shader> SpriteComponent::shader() const {
    return mSprite->shader();
}
