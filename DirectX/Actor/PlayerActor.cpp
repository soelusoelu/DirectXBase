﻿#include "PlayerActor.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"

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
}

void PlayerActor::updateActor() {
}
