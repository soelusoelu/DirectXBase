#pragma once

#include "../Utility/Collision.h"
#include "../Utility/Math.h"
#include <fstream>
#include <memory>
#include <string>

class Material;
struct MaterialData;
class Renderer;
class Texture;
class VertexArray;

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 norm;
    Vector2 tex;
};

class MeshLoader {
public:
    MeshLoader(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    ~MeshLoader();
    void createSphere(std::shared_ptr<Sphere>* sphere) const;
    void setVertexBuffer(unsigned numStream = 1, unsigned start = 0, unsigned offset = 0);
    void setIndexBuffer(unsigned index, unsigned offset = 0);
    unsigned getMaterialSize() const;
    std::shared_ptr<MaterialData> getMaterialData(unsigned index) const;

private:
    bool load(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    bool preload(std::ifstream& stream, std::shared_ptr<Renderer> renderer, const std::string& fileName); //事前に頂点数などを調べる

private:
    std::unique_ptr<Material> mMaterial;
    std::unique_ptr<VertexArray> mVertexArray;
};
