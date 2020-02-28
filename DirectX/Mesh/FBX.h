#pragma once

#include "../Utility/Math.h"
#include <fbxsdk.h>
#include <memory>

struct Vertex {
    Vector4 pos;
};

class VertexArray;

class FBX {
public:
    FBX();
    ~FBX();
    void initialize();

private:
    void enumNodeNamesAndAttributes(FbxNode* node, int indent);
    void searchIndex(FbxMesh* mesh);
    void searchVertex(FbxMesh* mesh);

private:
    FbxManager* mManager;
    std::unique_ptr<VertexArray> mVertexArray;
};
