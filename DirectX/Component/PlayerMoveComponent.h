#pragma once

#include "Component.h"
#include <memory>

class Actor;
class Renderer;
class SoundComponent;

class PlayerMoveComponent : public Component {
public:
    PlayerMoveComponent(std::shared_ptr<Actor> owner, int updateOrder = 10);
    ~PlayerMoveComponent();
    virtual void start() override;
    virtual void update() override;

private:
    void move();

private:
    std::shared_ptr<SoundComponent> mSound;
};

