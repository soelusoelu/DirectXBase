#pragma once

#include "Actor.h"
#include <memory>

class Renderer;

class FriedChicken : public Actor {
public:
    FriedChicken(std::shared_ptr<Renderer> renderer);
    ~FriedChicken();
    virtual void start() override;
    virtual void updateActor() override;
};
