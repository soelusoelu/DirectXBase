#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class Mesh;
class Renderer;
class Transform3D;

class DirectionalLight {
public:
    DirectionalLight();
    ~DirectionalLight();
    void update();
    void loadProperties(const rapidjson::Value& inObj);
    void createMesh(std::shared_ptr<Renderer> renderer);
    const Vector3& getDirection() const;
    void setDirection(const Vector3& dir);
    const Vector3& getColor() const;
    void setColor(const Vector3& color);

private:
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Transform3D> mTransform;
    Vector3 mDirection;
    Vector3 mColor;
};
