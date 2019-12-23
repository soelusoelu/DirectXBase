#pragma once

#include "Actor.h"
#include <memory>

class Renderer;
class CircleCollisionComponent;
class SpriteComponent;

class PlayerActor : public Actor {
public:
    PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag = "Player");
    ~PlayerActor();
    virtual void updateActor() override;

private:
    CircleCollisionComponent* mCollide;
    SpriteComponent* mSprite;
};

