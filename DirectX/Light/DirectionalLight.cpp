#include "DirectionalLight.h"
#include "../Actor/Transform3D.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../Mesh/Mesh.h"
#include "../Utility/LevelLoader.h"

DirectionalLight::DirectionalLight() :
    mMesh(nullptr),
    mTransform(std::make_shared<Transform3D>()),
    mDirection(Vector3::zero),
    mColor(Vector3::one) {
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::update() {
    mTransform->rotate(Vector3::right, Time::deltaTime * 30.f);
    mTransform->computeWorldTransform();

    mDirection = Vector3::transform(Vector3::up, mTransform->getRotation());
}

void DirectionalLight::loadProperties(const rapidjson::Value& inObj) {
    const auto& dirObj = inObj["directionalLight"];
    if (dirObj.IsObject()) {
        //向きと色を設定
        JsonHelper::getVector3(dirObj, "direction", &mDirection);
        JsonHelper::getVector3(dirObj, "color", &mColor);
    }
}

void DirectionalLight::createMesh(std::shared_ptr<Renderer> renderer) {
#ifdef _DEBUG //デバッグ時のみメッシュ描画
    mMesh = std::make_shared<Mesh>(renderer, "Light/DLight.obj");
    mMesh->addToManager();
    mMesh->setTransform(mTransform);
    //位置やスケールは計算には関係ない
    mTransform->setPosition(Vector3(0.f, 5.f, 0.f));
    mTransform->setScale(0.25f);
    mTransform->computeWorldTransform();
#endif // _DEBUG
}

const Vector3& DirectionalLight::getDirection() const {
    return mDirection;
}

void DirectionalLight::setDirection(const Vector3& dir) {
    mDirection = dir;
}

const Vector3& DirectionalLight::getColor() const {
    return mColor;
}

void DirectionalLight::setColor(const Vector3& color) {
    mColor = color;
}
