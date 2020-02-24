#pragma once

#include "Actor.h"
#include <memory>

class Renderer;

class PlayerActor : public Actor {
public:
    PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag = "Player");
    ~PlayerActor();
    virtual void start() override;
    virtual void updateActor() override;
};

