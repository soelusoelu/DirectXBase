#include "VertexArray.h"
#include "BufferDesc.h"
#include "DirectX.h"
#include "Game.h"
#include "IndexBuffer.h"
#include "SubResourceDesc.h"
#include "Usage.h"
#include "VertexBuffer.h"

VertexArray::VertexArray() :
    mNumVerts(0),
    mNumNormal(0),
    mNumTex(0),
    mNumFace(0),
    mVertices(nullptr),
    mVertexBuffer(nullptr) {
}

VertexArray::~VertexArray() {
    SAFE_DELETE_ARRAY(mVertices);
}

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

void VertexArray::setVertices(Vector3* vertices) {
    mVertices = vertices;
}

Vector3* VertexArray::getVertices() const {
    return mVertices;
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
    BufferDesc bd;
    bd.size = sizeof(int) * numFace * 3;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);
    SubResourceDesc sub;
    sub.data = data;

    mIndexBuffer[index] = std::make_unique<IndexBuffer>(bd, &sub);
}

void VertexArray::resizeIndexBuffer(size_t size) {
    mIndexBuffer.resize(size);
}

void VertexArray::setVertexBuffer(unsigned numStream, unsigned start, unsigned offset) {
    mVertexBuffer->setVertexBuffer(start, numStream, offset);
}

void VertexArray::setIndexBuffer(unsigned index, unsigned offset) {
    mIndexBuffer[index]->setIndexBuffer(Format::FORMAT_R32_UINT, offset);
}
