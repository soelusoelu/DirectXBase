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
    ALIGN16 Vector3 diffuseColor;
    ALIGN16 float innerRadius;
    ALIGN16 float outerRadius;
    ALIGN16 Vector2 windowSize;
};

struct PointLight {
    Mesh* mesh;
    std::shared_ptr<Shader> shader;

    PointLight();
    ~PointLight();
    void initialize(std::shared_ptr<Renderer> renderer);
};
