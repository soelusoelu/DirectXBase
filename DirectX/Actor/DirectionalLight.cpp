#include "DirectionalLight.h"
#include "Transform3D.h"
#include "../Device/Renderer.h"
#include "../Mesh/Mesh.h"

DirectionalLight::DirectionalLight(std::shared_ptr<Renderer> renderer) :
    mMesh(renderer->createMesh("DLight.obj")),
    mTransform(std::make_shared<Transform3D>())
{
    mMesh->setTransform(mTransform);
    mTransform->setPosition(Vector3(0.f, 3.f, 0.f));
    mTransform->rotate(Vector3::right, 180.f);
    mTransform->computeWorldTransform();
}

DirectionalLight::~DirectionalLight() {
    mMesh->destroy();
}

void DirectionalLight::update() {
    mTransform->rotate(Vector3::right, 0.5f);
    mTransform->computeWorldTransform();

    dir = Vector3::transform(Vector3::up, mTransform->getRotation());
}

Vector3 DirectionalLight::dir = Vector3::zero;
