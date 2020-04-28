#pragma once

#include "Component.h"
#include <memory>

class Sprite3D;

class BirdOrbit : public Component {
public:
    BirdOrbit(std::shared_ptr<GameObject> owner);
    ~BirdOrbit();
    virtual void start() override;
    void setActive(bool value);
    void setPositionZ(float z);

private:
    std::shared_ptr<Sprite3D> mSprite;
};
