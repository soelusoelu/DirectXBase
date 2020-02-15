#include "VertexArray.h"
#include "Buffer.h"
#include "BufferDesc.h"
#include "Game.h"
#include "SubResourceDesc.h"
#include "Usage.h"
#include "VertexStreamDesc.h"
#include "../Device/Renderer.h"

VertexArray::VertexArray(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
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
    bd.size = vertexSize * mNumVerts;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = data;

    if (auto r = mRenderer.lock()) {
        mVertexBuffer = r->createBuffer(bd, &sub);
    }
}

void VertexArray::createIndexBuffer(unsigned index, unsigned numFace, const void* data) {
    BufferDesc bd;
    bd.size = sizeof(int) * numFace * 3;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);
    SubResourceDesc sub;
    sub.data = data;

    if (auto r = mRenderer.lock()) {
        mIndexBuffer[index] = r->createBuffer(bd, &sub);
    }
}

std::shared_ptr<Buffer> VertexArray::getIndexBuffer(unsigned index) const {
    return mIndexBuffer[index];
}

void VertexArray::resizeIndexBuffer(size_t size) {
    mIndexBuffer.resize(size);
}

void VertexArray::setVertexBuffer(unsigned vertexSize, unsigned numStream, unsigned start) {
    VertexStreamDesc stream;
    stream.sharedBuffer = mVertexBuffer;
    stream.offset = 0;
    stream.stride = vertexSize;

    if (auto r = mRenderer.lock()) {
        r->setVertexBuffer(&stream, numStream, start);
    }
}

void VertexArray::setIndexBuffer(unsigned index, unsigned offset) {
    if (auto r = mRenderer.lock()) {
        r->setIndexBuffer(mIndexBuffer[index], offset);
    }
}
