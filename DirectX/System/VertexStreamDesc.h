#pragma once

#include <memory>

class Buffer;

struct VertexStreamDesc {
    //頂点バッファ
    std::shared_ptr<Buffer> buffer;
    //オフセット
    unsigned offset;
    //ストライド
    unsigned stride;

    VertexStreamDesc() :
        buffer(nullptr),
        offset(0),
        stride(0) {
    }
};
