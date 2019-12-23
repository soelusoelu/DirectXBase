#include "PlayerActor.h"
#include "../Component/CircleCollisionComponent.h"
#include "../Component/SpriteComponent.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(tag),
    mCollide(new CircleCollisionComponent(this)),
    mSprite(new SpriteComponent(this, renderer, "player_128.png", 0.5f)) {
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::updateActor() {
}
