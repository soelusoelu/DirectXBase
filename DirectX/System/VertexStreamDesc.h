#pragma once

#include <memory>

class Buffer;

struct VertexStreamDesc {
    //頂点バッファ
    Buffer* buffer;
    std::shared_ptr<Buffer> sharedBuffer;
    //オフセット
    unsigned offset;
    //ストライド
    unsigned stride;

    VertexStreamDesc() :
        buffer(nullptr),
        sharedBuffer(nullptr),
        offset(0),
        stride(0) {
    }
};
