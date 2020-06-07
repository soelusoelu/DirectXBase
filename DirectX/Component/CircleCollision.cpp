#include "CircleCollision.h"
#include "ComponentManager.h"
#include "SpriteComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform2D.h"

CircleCollision::CircleCollision() :
    Collider(),
    mSprite(nullptr),
    mCircle(std::make_unique<Circle>()) {
}

CircleCollision::~CircleCollision() = default;

void CircleCollision::start() {
    Collider::start();

    mSprite = owner()->componentManager()->getComponent<SpriteComponent>();
    circleUpdate();
}

void CircleCollision::update() {
    Collider::update();

    if (!mIsAutoUpdate) {
        return;
    }

    circleUpdate();
}

void CircleCollision::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    Collider::drawDebugInfo(inspect);

    inspect->emplace_back("Center", mCircle->center);
    inspect->emplace_back("Radius", mCircle->radius);
}

void CircleCollision::set(const Vector2& center, float radius) {
    mCircle->set(center, radius);
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Circle& CircleCollision::getCircle() const {
    return *mCircle;
}

void CircleCollision::circleUpdate() {
    if (!mSprite) {
        return;
    }

    const auto& t = mSprite->transform();
    const auto& size = mSprite->getTextureSize();
    auto maxSize = Math::Max<float>(size.x, size.y);
    const auto& scale = t->getScale();
    auto maxScale = Math::Max<float>(scale.x, scale.y);
    auto radius = maxSize / 2.f * maxScale;

    mCircle->set(t->getPosition(), radius);
}
