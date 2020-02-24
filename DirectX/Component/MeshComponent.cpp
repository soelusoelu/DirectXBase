#include "MeshComponent.h"
#include "../Mesh/Mesh.h"
#include "../Actor/Actor.h"
#include "../Device/Renderer.h"
#include "../Utility/LevelLoader.h"

MeshComponent::MeshComponent(std::shared_ptr<Actor> owner) :
    Component(owner),
    mMesh(nullptr) {
}

MeshComponent::~MeshComponent() {
    mMesh->destroy();
}

void MeshComponent::start() {
}

void MeshComponent::update() {
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string fileName;
    if (JsonHelper::getString(inObj, "meshFile", &fileName)) {
        setMesh(fileName);
    }
}

void MeshComponent::setMesh(const std::string& fileName) {
    if (mMesh) {
        mMesh->destroy();
    }
    mMesh = std::make_shared<Mesh>(owner()->renderer(), fileName);
    mMesh->addToManager();
    mMesh->setTransform(owner()->transform());
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
