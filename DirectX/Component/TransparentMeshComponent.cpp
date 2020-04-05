#include "TransparentMeshComponent.h"
#include "../GameObject/GameObject.h"
#include "../Mesh/Mesh.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

TransparentMeshComponent::TransparentMeshComponent(std::shared_ptr<GameObject> owner) :
    MeshComponent(owner, "TransparentMeshComponent") {
}

TransparentMeshComponent::~TransparentMeshComponent() = default;

void TransparentMeshComponent::loadProperties(const rapidjson::Value& inObj) {
    MeshComponent::loadProperties(inObj);

    float alpha;
    if (JsonHelper::getFloat(inObj, "alpha", &alpha)) {
        setAlpha(alpha);
    }
}

void TransparentMeshComponent::drawDebugInfo(debugInfoList* inspect) const {
    debugInfo info;
    info.first = "Alpha";
    info.second = StringUtil::floatToString(mMesh->getAlpha());
    inspect->emplace_back(info);
}

void TransparentMeshComponent::setMesh(const std::string& fileName) {
    mMesh = std::make_shared<Mesh>(fileName);
    mMesh->addToManager(true);
    mMesh->setTransform(owner()->transform());
}

void TransparentMeshComponent::setAlpha(float alpha) {
    mMesh->setAlpha(alpha);
}

float TransparentMeshComponent::getAlpha() const {
    return mMesh->getAlpha();
}
