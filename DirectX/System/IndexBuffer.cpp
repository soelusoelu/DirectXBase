#include "IndexBuffer.h"
#include "../Device/Renderer.h"

IndexBuffer::IndexBuffer(std::shared_ptr<Renderer> renderer, const BufferDesc& desc, const SubResourceDesc* data) :
    Buffer(renderer, desc, data),
    mRenderer(renderer) {
}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::setIndexBuffer(Format format, unsigned offset) {
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->IASetIndexBuffer(mBuffer, toFormat(format), offset);
    }
}
