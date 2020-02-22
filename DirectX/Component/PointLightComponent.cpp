﻿#include "PointLightComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Camera/Camera.h"
#include "../Device/Renderer.h"
#include "../Light/PointLight.h"
#include "../Mesh/Material.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshLoader.h"
#include "../Shader/Shader.h"
#include "../System/SubResourceDesc.h"

PointLightComponent::PointLightComponent(Actor* owner) :
    Component(owner),
    mDiffuseColor(ColorPalette::white),
    mInnerRadius(0.5f),
    mOuterRadius(1.f),
    mIntensity(1.f) {
    owner->renderer()->addPointLight(this);
}

PointLightComponent::~PointLightComponent() {
    mOwner->renderer()->removePointLight(this);
}

void PointLightComponent::start() {
}

void PointLightComponent::update() {
}

void PointLightComponent::draw(std::shared_ptr<PointLight> pointLight, std::shared_ptr<Camera> camera) const {
    auto scale = Matrix4::createScale(mOwner->transform()->getScale() * mOuterRadius / pointLight->radius);
    auto trans = Matrix4::createTranslation(mOwner->transform()->getPosition());
    auto world = scale * trans;

    auto mesh = pointLight->mesh;
    auto shader = pointLight->shader;

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (shader->map(&msrd)) {
        PointLightConstantBuffer cb;
        cb.wvp = world * camera->getView() * camera->getProjection();
        cb.wvp.transpose();
        cb.worldPos = mOwner->transform()->getPosition();
        cb.cameraPos = camera->getPosition();
        cb.windowSize = Vector2(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
        cb.diffuseColor = mDiffuseColor;
        cb.innerRadius = mInnerRadius;
        cb.outerRadius = mOuterRadius;
        cb.intensity = mIntensity;

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        shader->unmap();
    }

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < mesh->getMeshData()->getMaterialSize(); i++) {
        //使用されていないマテリアル対策
        if (mesh->getMeshData()->getMaterialData(i)->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mesh->getMeshData()->setIndexBuffer(i);

        //プリミティブをレンダリング
        mOwner->renderer()->drawIndexed(mesh->getMeshData()->getMaterialData(i)->numFace * 3);
    }
}

void PointLightComponent::setColor(const Vector3& color) {
    mDiffuseColor = color;
}

void PointLightComponent::setInnerRadius(float radius) {
    mInnerRadius = radius;
}

void PointLightComponent::setOuterRadius(float radius) {
    mOuterRadius = radius;
}

void PointLightComponent::setIntensity(float value) {
    mIntensity = value;
}
