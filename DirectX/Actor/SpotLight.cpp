#include "SpotLight.h"
#include "Transform3D.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../Mesh/Mesh.h"

SpotLight::SpotLight(std::shared_ptr<Renderer> renderer) :
    mMesh(renderer->createMesh("SLight.obj")),
    mTransform(std::make_shared<Transform3D>()) {
    mMesh->setTransform(mTransform);
    mTransform->setPosition(Vector3(0.f, 5.f, 0.f));
    mTransform->setScale(0.5f);
    mTransform->computeWorldTransform();
}

SpotLight::~SpotLight() {
    mMesh->destroy();
}

void SpotLight::update() {
    mTransform->translate(Vector3::forward * Time::deltaTime);
    mTransform->rotate(Vector3::right, Time::deltaTime * 30.f);
    mTransform->computeWorldTransform();

    position = mTransform->getPosition();
    rot = Matrix4::createFromQuaternion(mTransform->getRotation());
}

Vector3 SpotLight::position = Vector3::zero;
Matrix4 SpotLight::rot = Matrix4::identity;