#include "PointLight.h"
#include "../Device/AssetsManager.h"
#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"
#include "../System/InputElement.h"

PointLight::PointLight() :
    mesh(nullptr),
    shader(nullptr),
    radius(0.f) {
}

PointLight::~PointLight() = default;

void PointLight::initialize() {
    //メッシュ描画されるとだるいから自己管理
    mesh = std::make_shared<Mesh>("Shape/Sphere.obj");
    radius = mesh->getRadius();
    shader = Singleton<AssetsManager>::instance().createShader("PointLight.hlsl");

    //コンスタントバッファ生成
    shader->createConstantBuffer(sizeof(PointLightConstantBuffer));

    //インプットレイアウトの生成
    static constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    static constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    shader->createInputLayout(layout, numElements);
}
