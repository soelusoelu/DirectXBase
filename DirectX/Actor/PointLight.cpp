#include "PointLight.h"
#include "Transform3D.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../Mesh/Mesh.h"

PointLight::PointLight(std::shared_ptr<Renderer> renderer) :
    mMesh(renderer->createMesh("PLight.obj")),
    mTransform(std::make_shared<Transform3D>()) {
    mMesh->setTransform(mTransform);
    mTransform->setPosition(Vector3(0.f, 3.f, 0.f));
    mTransform->setScale(0.5f);
    mTransform->computeWorldTransform();
}

PointLight::~PointLight() {
    mMesh->destroy();
}

void PointLight::update() {
    mTransform->translate(Vector3::forward * Time::deltaTime);
    mTransform->computeWorldTransform();

    position = mTransform->getPosition();
}

Vector3 PointLight::position = Vector3::zero;
