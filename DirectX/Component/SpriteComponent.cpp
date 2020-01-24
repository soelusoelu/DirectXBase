#include "SpriteComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform2D.h"
#include "../Sprite/Sprite.h"

SpriteComponent::SpriteComponent(Actor* owner, const char* fileName) :
    Component(owner),
    mSprite(new Sprite(mOwner->renderer(), fileName, mOwner->transform())) {
}

SpriteComponent::~SpriteComponent() {
    Sprite::destroy(mSprite);
}

void SpriteComponent::start() {
}

void SpriteComponent::update() {
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

Vector4 SpriteComponent::getColor() const {
    return mSprite->getColor();
}

void SpriteComponent::setUV(float l, float t, float r, float b) {
    mSprite->setUV(l, t, r, b);
}

Vector4 SpriteComponent::getUV() const {
    return mSprite->getUV();
}

Vector2 SpriteComponent::getTextureSize() const {
    return mSprite->getTextureSize();
}

void SpriteComponent::setActive(bool value) {
    mSprite->setActive(value);
}

bool SpriteComponent::getActive() const {
    return mSprite->getActive();
}

SpriteState SpriteComponent::getState() const {
    return mSprite->getState();
}

std::shared_ptr<Texture> SpriteComponent::texture() const {
    return mSprite->texture();
}

std::shared_ptr<Shader> SpriteComponent::shader() const {
    return mSprite->shader();
}

const char* SpriteComponent::fileName() const {
    return mSprite->fileName();
}
