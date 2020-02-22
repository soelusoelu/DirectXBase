#pragma once

#include "../Utility/Math.h"
#include <memory>

class Mesh;
class Renderer;
class Transform3D;

class DirectionalLight {
public:
    DirectionalLight(std::shared_ptr<Renderer> renderer);
    ~DirectionalLight();
    void update();

public:
    static Vector3 direction;
    static Vector3 color;

private:
    Mesh* mMesh;
    std::shared_ptr<Transform3D> mTransform;
};
