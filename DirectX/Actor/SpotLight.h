#pragma once

#include "../Utility/Math.h"
#include <memory>

class Mesh;
class Renderer;
class Transform3D;

class SpotLight {
public:
    SpotLight(std::shared_ptr<Renderer> renderer);
    ~SpotLight();
    void update();

public:
    static Vector3 position;
    static Matrix4 rot;

private:
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Transform3D> mTransform;
};

