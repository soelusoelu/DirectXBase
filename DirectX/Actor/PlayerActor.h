#pragma once

#include "Actor.h"
#include "../Utility/Math.h"
#include <memory>

class Renderer;
class CircleCollisionComponent;
class SoundComponent;
class SpriteComponent;
class PlayerMoveComponent;

class PlayerActor : public Actor {
public:
    PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag = "Player");
    ~PlayerActor();
    virtual void updateActor() override;

private:
    CircleCollisionComponent* mCollide;
    SoundComponent* mSound;
    SpriteComponent* mSprite;
    PlayerMoveComponent* mMove;
};

