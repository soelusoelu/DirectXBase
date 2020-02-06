﻿#include "DirectionalLight.h"
#include "Transform3D.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../Mesh/Mesh.h"

DirectionalLight::DirectionalLight(std::shared_ptr<Renderer> renderer) :
    mMesh(renderer->createMesh("DLight.obj")),
    mTransform(std::make_shared<Transform3D>()) {
    mMesh->setTransform(mTransform);
    //位置やスケールは計算には関係ない
    mTransform->setPosition(Vector3(0.f, 5.f, 0.f));
    mTransform->setScale(0.5f);
    mTransform->computeWorldTransform();
}

DirectionalLight::~DirectionalLight() {
    mMesh->destroy();
}

void DirectionalLight::update() {
    mTransform->rotate(Vector3::right, Time::deltaTime * 30.f);
    mTransform->computeWorldTransform();

    direction = Vector3::transform(Vector3::up, mTransform->getRotation());
}

Vector3 DirectionalLight::direction = Vector3::zero;
