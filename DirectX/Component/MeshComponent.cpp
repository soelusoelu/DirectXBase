#include "MeshComponent.h"
#include "../Mesh/Mesh.h"
#include "../Actor/Actor.h"
#include "../Device/Renderer.h"

MeshComponent::MeshComponent(Actor* owner, const char* filename) :
    Component(owner) {
    mMesh = mOwner->renderer()->createMesh(filename);
    mMesh->setTransform(mOwner->transform());
}

MeshComponent::~MeshComponent() {
    mMesh->destroy();
}

void MeshComponent::start() {
}

void MeshComponent::update() {
}

Mesh* MeshComponent::getMesh() const {
    return mMesh;
}

void MeshComponent::setActive(bool value) {
    mMesh->setActive(value);
}

bool MeshComponent::getActive() const {
    return mMesh->getActive();
}
