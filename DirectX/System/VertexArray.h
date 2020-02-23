#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <vector>

class IndexBuffer;
class VertexBuffer;

class VertexArray {
    using IndexBufferPtr = std::unique_ptr<IndexBuffer>;
    using IndexBufferPtrArray = std::vector<IndexBufferPtr>;

public:
    VertexArray();
    ~VertexArray();

    void setNumVerts(unsigned num);
    unsigned getNumVerts() const;
    void setNumNormal(unsigned num);
    unsigned getNumNormal() const;
    void setNumTex(unsigned num);
    unsigned getNumTex() const;
    void setNumFace(unsigned num);
    unsigned getNumFace() const;

    void setVertices(Vector3* vertices);
    Vector3* getVertices() const;

    void createVertexBuffer(unsigned vertexSize, const void* data);
    void createIndexBuffer(unsigned index, unsigned numFace, const void* data);
    void resizeIndexBuffer(size_t size);

    void setVertexBuffer(unsigned numStream = 1, unsigned start = 0, unsigned offset = 0);
    void setIndexBuffer(unsigned index, unsigned offset = 0);

private:
    unsigned mNumVerts; //頂点数
    unsigned mNumNormal; //法線数
    unsigned mNumTex; //テクスチャ座標数
    unsigned mNumFace; //ポリゴン数

    Vector3* mVertices; //頂点情報

    std::unique_ptr<VertexBuffer> mVertexBuffer;
    IndexBufferPtrArray mIndexBuffer;
};
