#pragma once

#include "Actor.h"
#include <memory>

class PlayerMoveComponent;
class Renderer;

class PlayerActor : public Actor {
public:
    PlayerActor(std::shared_ptr<Renderer> renderer);
    ~PlayerActor();
    virtual void start() override;
    virtual void updateActor() override;
    bool isJump() const;
    void setTargetPosition(const Vector3& pos);

private:
    std::shared_ptr<PlayerMoveComponent> mMoveComp;
};

