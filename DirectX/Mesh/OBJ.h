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
class VertexArray;

class OBJ : public IMeshLoader {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    OBJ();
    ~OBJ();
    virtual void perse(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName) override;
    virtual std::shared_ptr<Material> getMaterial(unsigned index) const override;
    virtual std::shared_ptr<VertexArray> getVertexArray() const override;
    virtual size_t getNumMaterial() const override;
    virtual void createSphere(std::shared_ptr<Sphere>* sphere) const override;

private:
    bool preload(std::ifstream& stream, std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName); //事前に頂点数などを調べる
    bool materialLoad(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName);
    bool materialPreload(std::ifstream& stream, const std::string& fileName);

private:
    MaterialPtrArray mMaterials;
    std::shared_ptr<VertexArray> mVertexArray;
    MeshVertex* mVertices;
};
