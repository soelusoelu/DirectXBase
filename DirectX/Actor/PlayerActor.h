#pragma once

#include "Actor.h"
#include "../Utility/Math.h"
#include <memory>

class Renderer;
class SoundComponent;
class PlayerMoveComponent;

class PlayerActor : public Actor {
public:
    PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag = "Player");
    ~PlayerActor();
    virtual void updateActor() override;

private:
    SoundComponent* mSound;
    PlayerMoveComponent* mMove;
};

