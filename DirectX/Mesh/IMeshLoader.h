#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>
#include <vector>

struct Material;
class VertexArray;

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

class IMeshLoader {
protected:
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    virtual ~IMeshLoader() {};
    virtual void perse(const std::string& fileName) = 0;
    virtual void setInitMaterials(MaterialPtrArray* rhs) const = 0;
    virtual std::shared_ptr<VertexArray> getVertexArray() const = 0;
    virtual Vector3 getCenter() const = 0;
    virtual float getRadius() const = 0;
};
