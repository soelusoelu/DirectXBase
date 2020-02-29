#pragma once

#include "../Utility/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>

struct Vertex {
    Vector4 pos;
    Vector4 normal;
    Vector2 uv;
};

class VertexArray;

class FBX {
public:
    FBX();
    ~FBX();
    void create(const std::string& fileName);
    std::shared_ptr<VertexArray> getVertexArray() const;

private:
    void perse(FbxNode* node, int indent);
    void getVertex(FbxMesh* mesh);
    void searchIndex(FbxMesh* mesh);
    void getNormals(FbxMesh* mesh);
    void getUV(FbxMesh* mesh);

private:
    FbxManager* mManager;
    Vertex* mVertices;
    std::shared_ptr<VertexArray> mVertexArray;
};
