#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <vector>

class Buffer;
class BufferDesc;
class Renderer;
class SubResourceDesc;
class VertexStreamDesc;

class VertexArray {
public:
    VertexArray(std::shared_ptr<Renderer> renderer);
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
    std::shared_ptr<Buffer> getIndexBuffer(unsigned index) const;
    void resizeIndexBuffer(size_t size);

    void setVertexBuffer(unsigned vertexSize, unsigned numStream = 1, unsigned start = 0);
    void setIndexBuffer(unsigned index, unsigned offset = 0);

private:
    std::weak_ptr<Renderer> mRenderer;

    unsigned mNumVerts; //頂点数
    unsigned mNumNormal; //法線数
    unsigned mNumTex; //テクスチャ座標数
    unsigned mNumFace; //ポリゴン数

    Vector3* mVertices; //頂点情報

    std::shared_ptr<Buffer> mVertexBuffer;
    std::vector<std::shared_ptr<Buffer>> mIndexBuffer;
};
