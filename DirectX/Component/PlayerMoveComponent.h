#pragma once

#include "Component.h"
#include "../Utility/Input.h"
#include "../Utility/Math.h"
#include <memory>

class Actor;
class Renderer;
class CircleCollisionComponent;
class SoundComponent;

class PlayerMoveComponent : public Component {
public:
    PlayerMoveComponent(Actor* owner, int updateOrder = 10);
    ~PlayerMoveComponent();
    virtual void start() override;
    virtual void update() override;

private:
    void move();

private:
    std::shared_ptr<CircleCollisionComponent> mCollider;
    std::shared_ptr<SoundComponent> mSound;
};

