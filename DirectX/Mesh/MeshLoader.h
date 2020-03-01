#pragma once

#include "IMeshLoader.h"
#include "../Utility/Collision.h"
#include "../Utility/Math.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class AssetsManager;
struct Material;
class Texture;
class VertexArray;

class MeshLoader {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    MeshLoader(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName);
    ~MeshLoader();
    void createSphere(std::shared_ptr<Sphere>* sphere) const;
    void setVertexBuffer(unsigned numStream = 1, unsigned start = 0, unsigned offset = 0);
    void setIndexBuffer(unsigned index, unsigned offset = 0);
    unsigned getNumMaterial() const;
    std::shared_ptr<Material> getMaterialData(unsigned index) const;

private:
    bool load(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName);
    bool preload(std::ifstream& stream, std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName); //事前に頂点数などを調べる
    bool materialLoad(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName);
    bool materialPreload(std::ifstream& stream, const std::string& fileName);

private:
    MaterialPtrArray mMaterials;
    std::unique_ptr<VertexArray> mVertexArray;
};
