#include "VertexBuffer.h"
#include "../Device/Renderer.h"

VertexBuffer::VertexBuffer(std::shared_ptr<Renderer> renderer, const BufferDesc& desc, const SubResourceDesc* data) :
    Buffer(renderer, desc, data),
    mRenderer(renderer) {
}

VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::setVertexBuffer(unsigned start, unsigned numStream, unsigned offset) {
    /* IASetVertexBuffers
        使い始めのスロット番号
        頂点バッファ配列の要素数
        頂点バッファ配列の先頭ポインタ
        INPUT_ELEMENT_DESC構造体のサイズが入った配列への先頭ポインタ(stride(読み込み単位)として扱うため)
        頂点バッファ配列の各頂点バッファの頭出しをするオフセット値の配列
    */
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->IASetVertexBuffers(start, numStream, &mBuffer, &mDesc.oneSize, &offset);
    }
}
