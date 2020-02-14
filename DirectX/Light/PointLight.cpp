#include "PointLight.h"
#include "../Device/Renderer.h"
#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"
#include "../System/InputElement.h"

PointLight::PointLight() :
    mesh(nullptr),
    shader(nullptr) {
}

PointLight::~PointLight() = default;

void PointLight::initialize(std::shared_ptr<Renderer> renderer) {
    mesh = new Mesh(renderer, "Sphere.obj");
    shader = std::make_shared<Shader>(renderer, "PointLight.hlsl");

    //コンスタントバッファ生成
    shader->createConstantBuffer(renderer, sizeof(PointLightConstantBuffer));

    //インプットレイアウトの生成
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    shader->createInputLayout(layout, numElements);
}
