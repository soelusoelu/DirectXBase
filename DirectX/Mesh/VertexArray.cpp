#include "VertexArray.h"
#include "../DirectX/BufferDesc.h"
#include "../DirectX/DirectX.h"
#include "../DirectX/IndexBuffer.h"
#include "../DirectX/SubResourceDesc.h"
#include "../DirectX/Usage.h"
#include "../DirectX/VertexBuffer.h"

VertexArray::VertexArray() :
    mNumVerts(0),
    mNumNormal(0),
    mNumTex(0),
    mNumFace(0),
    mVertexBuffer(nullptr) {
}

VertexArray::~VertexArray() = default;

void VertexArray::setNumVerts(unsigned num) {
    mNumVerts = num;
}

unsigned VertexArray::getNumVerts() const {
    return mNumVerts;
}

void VertexArray::setNumNormal(unsigned num) {
    mNumNormal = num;
}

unsigned VertexArray::getNumNormal() const {
    return mNumNormal;
}

void VertexArray::setNumTex(unsigned num) {
    mNumTex = num;
}

unsigned VertexArray::getNumTex() const {
    return mNumTex;
}

void VertexArray::setNumFace(unsigned num) {
    mNumFace = num;
}

unsigned VertexArray::getNumFace() const {
    return mNumFace;
}

void VertexArray::createVertexBuffer(unsigned vertexSize, const void* data) {
    BufferDesc bd;
    bd.oneSize = vertexSize;
    bd.size = vertexSize * mNumVerts;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = data;

    mVertexBuffer = std::make_unique<VertexBuffer>(bd, &sub);
}

void VertexArray::createIndexBuffer(unsigned index, unsigned numFace, const void* data) {
    auto num = mIndexBuffer.size();
    if (index >= num) {
        mIndexBuffer.resize(num + 1);
    }

    BufferDesc bd;
    bd.size = sizeof(int) * numFace;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);
    SubResourceDesc sub;
    sub.data = data;

    mIndexBuffer[index] = std::make_unique<IndexBuffer>(bd, &sub);
}

void VertexArray::setVertexBuffer(unsigned numStream, unsigned start, unsigned offset) {
    mVertexBuffer->setVertexBuffer(start, numStream, offset);
}

void VertexArray::setIndexBuffer(unsigned index, unsigned offset) {
    mIndexBuffer[index]->setIndexBuffer(Format::FORMAT_R32_UINT, offset);
}
