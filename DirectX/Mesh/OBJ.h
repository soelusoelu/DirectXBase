#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
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
    virtual void perse(std::shared_ptr<AssetsManager> assetsManager, const std::string& filePath) override;
    virtual std::shared_ptr<Material> getMaterial(unsigned index) const override;
    virtual std::shared_ptr<VertexArray> getVertexArray() const override;
    virtual size_t getNumMaterial() const override;
    virtual Vector3 getCenter() const override;
    virtual float getRadius() const override;

private:
    bool preload(std::ifstream& stream, std::shared_ptr<AssetsManager> assetsManager, const std::string& filePath); //事前に頂点数などを調べる
    bool materialLoad(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName, const std::string& filePath);
    void materialPreload(std::ifstream& stream);

private:
    MaterialPtrArray mMaterials;
    std::shared_ptr<VertexArray> mVertexArray;
    MeshVertex* mVertices;
};
