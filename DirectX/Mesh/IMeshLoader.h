#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>

class AssetsManager;
struct Material;
class VertexArray;

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

class IMeshLoader {
public:
    virtual ~IMeshLoader() {};
    virtual void perse(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName) = 0;
    virtual std::shared_ptr<Material> getMaterial(unsigned index) const = 0;
    virtual std::shared_ptr<VertexArray> getVertexArray() const = 0;
    virtual size_t getNumMaterial() const = 0;
    virtual Vector3 getCenter() const = 0;
    virtual float getRadius() const = 0;
};
