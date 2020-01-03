#include "SpriteComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform2D.h"
#include "../Sprite/Sprite.h"

SpriteComponent::SpriteComponent(Actor* owner, std::shared_ptr<Renderer> renderer, const char* fileName, float z) :
    Component(owner),
    mSprite(new Sprite(renderer, fileName, z, false)) {
    mOwner->getTransform()->setPrimary(z);
    mOwner->getTransform()->setPivot(mSprite->getPivot());
}

SpriteComponent::~SpriteComponent() {
    Sprite::destroy(mSprite);
}

void SpriteComponent::start() {
}

void SpriteComponent::update() {
}

void SpriteComponent::onUpdateWorldTransform() {
    mSprite->setWorld(mOwner->getTransform()->getWorldTransform());
}

Sprite* SpriteComponent::getSprite() const {
    return mSprite;
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
    mOwner->getTransform()->setPivot(mSprite->getPivot());
}

Vector4 SpriteComponent::getUV() const {
    return mSprite->getUV();
}

Vector2INT SpriteComponent::getTextureSize() const {
    return mSprite->getTextureSize();
}

Vector2INT SpriteComponent::getCurrentTextureSize() const {
    return mSprite->getCurrentTextureSize();
}

Vector2INT SpriteComponent::getScreenTextureSize() const {
    return Vector2INT(
        mSprite->getCurrentTextureSize().x * mOwner->getTransform()->getScale().x,
        mSprite->getCurrentTextureSize().y * mOwner->getTransform()->getScale().y
    );
}

SpriteState SpriteComponent::getState() const {
    return mSprite->getState();
}

void SpriteComponent::setTexture(const char* fileName) {
    mSprite->setTexture(fileName);
    mOwner->getTransform()->setPivot(mSprite->getPivot());
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
