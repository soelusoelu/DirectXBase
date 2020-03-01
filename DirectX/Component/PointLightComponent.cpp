#include "PointLightComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Camera/Camera.h"
#include "../Device/Renderer.h"
#include "../Light/PointLight.h"
#include "../Mesh/Material.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/OBJ.h"
#include "../Mesh/VertexArray.h"
#include "../Shader/Shader.h"
#include "../System/DirectX.h"
#include "../System/SubResourceDesc.h"
#include "../Utility/LevelLoader.h"

PointLightComponent::PointLightComponent(std::shared_ptr<Actor> owner) :
    Component(owner, "PointLightComponent"),
    mColor(ColorPalette::white),
    mInnerRadius(0.5f),
    mOuterRadius(1.f),
    mIntensity(1.f) {
}

PointLightComponent::~PointLightComponent() = default;

void PointLightComponent::start() {
    owner()->renderer()->addPointLight(shared_from_this());
}

void PointLightComponent::update() {
}

void PointLightComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    Vector3 color;
    if (JsonHelper::getVector3(inObj, "color", &color)) {
        mColor = color;
    }

    float f;
    if (JsonHelper::getFloat(inObj, "innerRadius", &f)) {
        mInnerRadius = f;
    }
    if (JsonHelper::getFloat(inObj, "outerRadius", &f)) {
        mOuterRadius = f;
    }
    if (JsonHelper::getFloat(inObj, "intensity", &f)) {
        mIntensity = f;
    }
}

void PointLightComponent::draw(std::shared_ptr<PointLight> pointLight, std::shared_ptr<Camera> camera) const {
    auto scale = Matrix4::createScale(owner()->transform()->getScale() * mOuterRadius / pointLight->radius);
    auto trans = Matrix4::createTranslation(owner()->transform()->getPosition());
    auto world = scale * trans;

    auto mesh = pointLight->mesh;
    auto shader = pointLight->shader;

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (shader->map(&msrd)) {
        PointLightConstantBuffer cb;
        cb.wvp = world * camera->getView() * camera->getProjection();
        cb.wvp.transpose();
        cb.worldPos = owner()->transform()->getPosition();
        cb.cameraPos = camera->getPosition();
        cb.windowSize = Vector2(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
        cb.diffuseColor = mColor;
        cb.innerRadius = mInnerRadius;
        cb.outerRadius = mOuterRadius;
        cb.intensity = mIntensity;

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        shader->unmap();
    }

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < mesh->getMeshData()->getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        if (mesh->getMeshData()->getMaterial(i)->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mesh->getMeshData()->getVertexArray()->setIndexBuffer(i);

        //プリミティブをレンダリング
        Singleton<DirectX>::instance().drawIndexed(mesh->getMeshData()->getMaterial(i)->numFace * 3);
    }
}

void PointLightComponent::setColor(const Vector3& color) {
    mColor = color;
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
