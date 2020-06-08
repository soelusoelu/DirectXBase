#include "PointLightComponent.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Device/Renderer.h"
#include "../Light/PointLight.h"
#include "../Mesh/Material.h"
#include "../Mesh/OBJ.h"
#include "../Mesh/VertexArray.h"
#include "../System/DirectX.h"
#include "../System/Shader.h"
#include "../System/SubResourceDesc.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

PointLightComponent::PointLightComponent() :
    Component(),
    mCamera(nullptr),
    mColor(ColorPalette::white),
    mInnerRadius(0.5f),
    mOuterRadius(1.f),
    mIntensity(1.f) {
}

PointLightComponent::~PointLightComponent() = default;

void PointLightComponent::start() {
    auto camera = owner()->getGameObjectManager()->find("Camera");
    mCamera = camera->componentManager()->getComponent<Camera>();
    owner()->renderer()->addPointLight(shared_from_this());
}

void PointLightComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector3(inObj, "color", &mColor);
    JsonHelper::getFloat(inObj, "innerRadius", &mInnerRadius);
    JsonHelper::getFloat(inObj, "outerRadius", &mOuterRadius);
    JsonHelper::getFloat(inObj, "intensity", &mIntensity);
}

void PointLightComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Color", mColor);
    inspect->emplace_back("InnerRadius", mInnerRadius);
    inspect->emplace_back("OuterRadius", mOuterRadius);
    inspect->emplace_back("Intensity", mIntensity);
}

void PointLightComponent::draw(const PointLight& pointLight) const {
    auto scale = Matrix4::createScale(owner()->transform()->getScale() * mOuterRadius / pointLight.radius);
    auto trans = Matrix4::createTranslation(owner()->transform()->getPosition());
    auto world = scale * trans;

    auto shader = pointLight.shader;

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (shader->map(&msrd)) {
        PointLightConstantBuffer cb;
        cb.wvp = world * mCamera->getViewProjection();
        cb.wvp.transpose();
        cb.worldPos = owner()->transform()->getPosition();
        cb.cameraPos = mCamera->owner()->transform()->getPosition();
        cb.windowSize = Vector2(Window::width(), Window::height());
        cb.diffuseColor = mColor;
        cb.innerRadius = mInnerRadius;
        cb.outerRadius = mOuterRadius;
        cb.intensity = mIntensity;

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        shader->unmap();
    }

    auto mesh = pointLight.mesh;
    auto mats = pointLight.materials;
    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < mats.size(); i++) {
        //使用されていないマテリアル対策
        if (mats[i]->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mesh->getVertexArray()->setIndexBuffer(i);

        //プリミティブをレンダリング
        Singleton<DirectX>::instance().drawIndexed(mats[i]->numFace * 3);
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
