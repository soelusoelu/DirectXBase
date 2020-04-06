#include "ChickenMeshComponent.h"

ChickenMeshComponent::ChickenMeshComponent(std::shared_ptr<GameObject> owner) :
    MeshComponent(owner, "ChickenMeshComponent") {
}

ChickenMeshComponent::~ChickenMeshComponent() = default;