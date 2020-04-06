#include "MeshComponent.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Mesh/Mesh.h"
#include "../Utility/LevelLoader.h"

MeshComponent::MeshComponent(std::shared_ptr<GameObject> owner, const std::string& type) :
    Component(owner, type),
    mMesh(nullptr) {
}

MeshComponent::~MeshComponent() {
    mMesh->destroy();
}

void MeshComponent::onSetActive(bool value) {
    setActive(value);
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string fileName;
    if (JsonHelper::getString(inObj, "fileName", &fileName)) {
        setMesh(fileName);
    }
}

void MeshComponent::setMesh(const std::string& fileName) {
    mMesh = std::make_shared<Mesh>(fileName);
    mMesh->addToManager();
    mMesh->setTransform(owner()->transform());
}

const Vector3& MeshComponent::getCenter() const {
    return mMesh->getCenter();
}

float MeshComponent::getRadius() const {
    return mMesh->getRadius();
}

void MeshComponent::setActive(bool value) {
    mMesh->setActive(value);
}

bool MeshComponent::getActive() const {
    return mMesh->getActive();
}

void MeshComponent::setColor(const Vector3& color) {
    mMesh->setColor(color);
}

const Vector3& MeshComponent::getColor() const {
    return mMesh->getColor();
}
