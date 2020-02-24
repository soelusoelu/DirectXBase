#pragma once

#include "Component.h"
#include <memory>

class Actor;

class SoundComponent : public Component {
public:
    SoundComponent(std::shared_ptr<Actor> owner);
    ~SoundComponent();
    virtual void start() override;
    virtual void update() override;
    void playOneShot(const char* fileName, float volumeScale = 1.f);
};

