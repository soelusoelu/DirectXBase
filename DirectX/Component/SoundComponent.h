#pragma once

#include "Component.h"
#include <memory>

class GameObject;

class SoundComponent : public Component {
public:
    SoundComponent(std::shared_ptr<GameObject> owner);
    ~SoundComponent();
    void playOneShot(const char* fileName, float volumeScale = 1.f);
};

