#pragma once

struct SubResourceDesc {
    //初期化データへのポインタ
    const void* data;
    //テクスチャのみで使用
    //テクスチャの1行の先頭から次の行までの距離
    unsigned pitch;
    //3Dテクスチャのみで使用
    //ある深さレベルの開始から次の深さまでの距離
    unsigned slicePitch;

    SubResourceDesc() :
        data(nullptr),
        pitch(0),
        slicePitch(0) {
    }
};

struct MappedSubResourceDesc {
    //データへのポインタ
    void* data;
    //データの行ピッチ
    unsigned rowPitch;
    //データの深さピッチ
    unsigned depthPitch;

    MappedSubResourceDesc() :
        data(nullptr),
        rowPitch(0),
        depthPitch(0) {
    }
};
