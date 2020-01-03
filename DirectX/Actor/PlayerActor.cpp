#include "PlayerActor.h"
#include "../Component/CircleCollisionComponent.h"
#include "../Component/SpriteComponent.h"
#include "Transform2D.h"
#include "../Device/Time.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(tag),
    mCollide(new CircleCollisionComponent(this)),
    mSprite(new SpriteComponent(this, renderer, "kiparupa_anm.png", 0.5f)) {
    mSprite->setUV(0.25f, 0.25f, 0.5f, 0.5f);
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::updateActor() {
    static float counter = 0.f;
    counter += Time::deltaTime * 100.f;
    getTransform()->setScale(Math::abs(Math::cos(counter)));
    getTransform()->rotate(3.f);
}
