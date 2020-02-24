#pragma once

#include "Actor.h"
#include <memory>

class Renderer;

class EmptyActor : public Actor {
public:
    EmptyActor(std::shared_ptr<Renderer> renderer);
    ~EmptyActor();
    virtual void start() override;
    virtual void updateActor() override;
};
