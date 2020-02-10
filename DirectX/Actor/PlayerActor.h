#pragma once

#include "Actor.h"
#include "../Utility/Math.h"
#include <memory>

class Renderer;
class MeshComponent;
class SoundComponent;
class SphereCollisionComponent;
class PlayerMoveComponent;

class PlayerActor : public Actor {
public:
    PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag = "Player");
    ~PlayerActor();
    virtual void updateActor() override;

private:
    MeshComponent* mMesh;
    SoundComponent* mSound;
    SphereCollisionComponent* mSphere;
    PlayerMoveComponent* mMove;
};

