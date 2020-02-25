#pragma once

#include "Actor.h"
#include <memory>

class Renderer;

class Field : public Actor {
public:
    Field(std::shared_ptr<Renderer> renderer);
    ~Field();
    virtual void start() override;
    virtual void updateActor() override;
};

