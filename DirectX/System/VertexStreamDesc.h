#pragma once

#include <memory>

class Buffer;

struct VertexStreamDesc {
    //頂点バッファ
    Buffer* buffer;
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
