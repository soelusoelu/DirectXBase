#pragma once

#include "IMeshLoader.h"
#include "../Utility/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>
#include <vector>

class AssetsManager;
struct Material;
class VertexArray;

class FBX : public IMeshLoader {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    FBX();
    ~FBX();
    virtual void perse(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName) override;
    virtual std::shared_ptr<Material> getMaterial(unsigned index) const override;
    virtual std::shared_ptr<VertexArray> getVertexArray() const override;
    virtual size_t getNumMaterial() const override;

private:
    void perse(std::shared_ptr<AssetsManager> assets, FbxNode* node, int indent);
    void getVertex(FbxMesh* mesh);
    void getNormals(FbxMesh* mesh);
    void getUV(FbxMesh* mesh);
    void getMaterial(std::shared_ptr<AssetsManager> assets, FbxMesh* mesh);

private:
    FbxManager* mManager;
    MeshVertex* mVertices;
    MaterialPtrArray mMaterials;
    std::shared_ptr<VertexArray> mVertexArray;
};
