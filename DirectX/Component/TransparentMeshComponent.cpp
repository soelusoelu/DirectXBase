#include "TransparentMeshComponent.h"
#include "../GameObject/GameObject.h"
#include "../Mesh/Mesh.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

TransparentMeshComponent::TransparentMeshComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "TransparentMeshComponent"),
    mMesh(nullptr) {
}

TransparentMeshComponent::~TransparentMeshComponent() {
    mMesh->destroy();
}

void TransparentMeshComponent::onSetActive(bool value) {
    setActive(value);
}

void TransparentMeshComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string fileName;
    if (JsonHelper::getString(inObj, "fileName", &fileName)) {
        setMesh(fileName);
    }
    float alpha;
    if (JsonHelper::getFloat(inObj, "alpha", &alpha)) {
        setAlpha(alpha);
    }
}

void TransparentMeshComponent::drawDebugInfo(debugInfoList* inspect) const {
    debugInfo info;
    info.first = "alpha";
    info.second = StringUtil::floatToString(mMesh->getAlpha());
    inspect->emplace_back(info);
}

void TransparentMeshComponent::setMesh(const std::string& fileName) {
    mMesh = std::make_shared<Mesh>(owner()->renderer(), fileName);
    mMesh->addToManager(true);
    mMesh->setTransform(owner()->transform());
}

const Vector3& TransparentMeshComponent::getCenter() const {
    return mMesh->getCenter();
}

float TransparentMeshComponent::getRadius() const {
    return mMesh->getRadius();
}

void TransparentMeshComponent::setAlpha(float alpha) {
    mMesh->setAlpha(alpha);
}

float TransparentMeshComponent::getAlpha() const {
    return mMesh->getAlpha();
}

void TransparentMeshComponent::setActive(bool value) {
    mMesh->setActive(value);
}

bool TransparentMeshComponent::getActive() const {
    return mMesh->getActive();
}
