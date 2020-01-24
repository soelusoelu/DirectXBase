#pragma once

#include "Component.h"

class Actor;

class SoundComponent : public Component {
public:
    SoundComponent(Actor* owner);
    ~SoundComponent();
    virtual void start() override;
    virtual void update() override;
    void playOneShot(const char* fileName, float volumeScale = 1.f);
};

