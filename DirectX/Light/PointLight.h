#pragma once

#include "../System/Game.h"
#include "../Utility/Math.h"
#include <memory>

class Mesh;
class Renderer;
class Shader;

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
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
    float radius;

    PointLight();
    ~PointLight();
    void initialize(std::shared_ptr<Renderer> renderer);
};
