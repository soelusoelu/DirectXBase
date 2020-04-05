#pragma once

#include "Component.h"
#include <memory>
#include <string>

class GameObject;

class SoundComponent : public Component {
public:
    SoundComponent(std::shared_ptr<GameObject> owner);
    ~SoundComponent();
    void playOneShot(const std::string& fileName, float volumeScale = 1.f);
};

