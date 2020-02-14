#include "PlayerActor.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(renderer, tag),
    mMesh(new MeshComponent(this, "Chips.obj")),
    mSound(new SoundComponent(this)),
    mSphere(new SphereCollisionComponent(this)),
    mMove(new PlayerMoveComponent(this))
    ,
    mPointLight(new PointLightComponent(this))
{
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::updateActor() {
}
