#pragma once

#include "../Utility/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>
#include <vector>

struct Vertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

class AssetsManager;
struct Material;
class VertexArray;

class FBX {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    FBX();
    ~FBX();
    void create(std::shared_ptr<AssetsManager> assets, const std::string& fileName);
    MaterialPtr getMaterial(unsigned index) const;
    std::shared_ptr<VertexArray> getVertexArray() const;
    size_t getNumMaterial() const;

private:
    void perse(std::shared_ptr<AssetsManager> assets, FbxNode* node, int indent);
    void getVertex(FbxMesh* mesh);
    void getNormals(FbxMesh* mesh);
    void getUV(FbxMesh* mesh);
    void getMaterial(std::shared_ptr<AssetsManager> assets, FbxMesh* mesh);

private:
    FbxManager* mManager;
    Vertex* mVertices;
    MaterialPtrArray mMaterials;
    std::shared_ptr<VertexArray> mVertexArray;
};
