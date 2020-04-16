#pragma once

#include "../Math/Math.h"
#include "../System/Game.h"
#include <memory>
#include <vector>

class IMeshLoader;
class Shader;
struct Material;

struct PointLightConstantBuffer {
    ALIGN16 Matrix4 wvp;
    ALIGN16 Vector3 worldPos;
    ALIGN16 Vector3 cameraPos;
    ALIGN16 Vector2 windowSize;
    ALIGN16 Vector3 diffuseColor;
    ALIGN16 float innerRadius;
    ALIGN16 float outerRadius;
    ALIGN16 float intensity;
};

struct PointLight {
    std::shared_ptr<IMeshLoader> mesh;
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Shader> shader;
    float radius;

    PointLight();
    ~PointLight();
    void initialize();
};
