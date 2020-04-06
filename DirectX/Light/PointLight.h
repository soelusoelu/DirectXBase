﻿#pragma once

#include "../Math/Math.h"
#include "../System/Game.h"
#include <memory>

class IMeshLoader;
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
    std::shared_ptr<IMeshLoader> mesh;
    std::shared_ptr<Shader> shader;
    float radius;

    PointLight();
    ~PointLight();
    void initialize();
};
