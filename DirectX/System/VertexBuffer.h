#pragma once

#include "Buffer.h"
#include "BufferDesc.h"
#include "SubResourceDesc.h"
#include <memory>

class Renderer;

class VertexBuffer : public Buffer {
public:
    VertexBuffer(std::shared_ptr<Renderer> renderer, const BufferDesc& desc, const SubResourceDesc* data);
    ~VertexBuffer();
    void setVertexBuffer(unsigned start = 0, unsigned numStream = 1, unsigned offset = 0);

private:
    std::weak_ptr<Renderer> mRenderer;
};
