#pragma once

#include "../Utility/Math.h"
#include <memory>

class Mesh;
class Renderer;
class Transform3D;

class PointLight {
public:
    PointLight(std::shared_ptr<Renderer> renderer);
    ~PointLight();
    void update();

public:
    static Vector3 position;

private:
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Transform3D> mTransform;
};

