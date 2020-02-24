#include "MeshComponent.h"
#include "../Mesh/Mesh.h"
#include "../Actor/Actor.h"
#include "../Device/Renderer.h"

MeshComponent::MeshComponent(Actor* owner, const std::string& filename) :
    Component(owner) {
    mMesh = std::make_shared<Mesh>(mOwner->renderer(), filename);
    mMesh->addToManager();
    mMesh->setTransform(mOwner->transform());
}

MeshComponent::~MeshComponent() {
    mMesh->destroy();
}

void MeshComponent::start() {
}

void MeshComponent::update() {
}

std::shared_ptr<Mesh> MeshComponent::getMesh() const {
    return mMesh;
}

void MeshComponent::setActive(bool value) {
    mMesh->setActive(value);
}

bool MeshComponent::getActive() const {
    return mMesh->getActive();
}
