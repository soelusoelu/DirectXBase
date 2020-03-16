#include "PlayerActor.h"
#include "Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Log.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer) :
    Actor(renderer, "Player") {
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::start() {
    //auto mesh = std::make_shared<MeshComponent>(shared_from_this());
    //mesh->setMesh("Chips.fbx");
    //mComponentManager->addComponent(mesh);
    mComponentManager->addComponent<SoundComponent>();
    //mComponentManager->addComponent<SphereCollisionComponent>();
    mComponentManager->addComponent<PlayerMoveComponent>();
    mComponentManager->addComponent<PointLightComponent>();

    Debug::log()->log("Player spawn.");
}

void PlayerActor::updateActor() {
    mTransform->rotate(Vector3::up, 0.5f);
}
