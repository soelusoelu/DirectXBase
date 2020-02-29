#include "PlayerActor.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "Transform3D.h"
#include "../Utility/Input.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer) :
    Actor(renderer, "Player") {
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::start() {
    mTransform->setScale(5.f);
    auto mesh = std::make_shared<MeshComponent>(shared_from_this());
    mesh->setMesh("Chips.fbx");
    mComponentManager->addComponent(mesh);
    //mComponentManager->addComponent(std::make_shared<SoundComponent>(shared_from_this()));
    //mComponentManager->addComponent(std::make_shared<SphereCollisionComponent>(shared_from_this()));
    mComponentManager->addComponent(std::make_shared<PlayerMoveComponent>(shared_from_this()));
    //mComponentManager->addComponent(std::make_shared<PointLightComponent>(shared_from_this()));
}

void PlayerActor::updateActor() {
    if (Input::getKey(KeyCode::Q)) {
        mTransform->rotate(Vector3::up, 3.f);
    }
}
