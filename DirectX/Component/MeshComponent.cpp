#include "MeshComponent.h"
#include "../Mesh/Mesh.h"
#include "../Actor/Actor.h"
#include "../Device/Renderer.h"

MeshComponent::MeshComponent(Actor* owner, const char* filename) :
    Component(owner) {
    mMesh = mOwner->renderer()->createMesh(filename);
    mMesh->setTransform(mOwner->transform());
}

void MeshComponent::start() {
}

void MeshComponent::update() {
}

std::shared_ptr<Mesh> MeshComponent::getMesh() const {
    return mMesh;
}
