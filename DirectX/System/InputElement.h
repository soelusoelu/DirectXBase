#pragma once

#include "InputElementDesc.h"
#include "VertexStreamDesc.h"
#include "DirectXIncLib.h"
#include <vector>
#include <memory>

class InputElement {
public:
    InputElement(ID3D11Device* device, const InputElementDesc* desc, unsigned numElements, ID3D10Blob* compile);
    ~InputElement();
    //ディスクリプタの要素数を返す
    size_t size() const;
    //ディスクリプタの取得
    const InputElementDesc& desc(unsigned index) const;
    //頂点レイアウトの取得
    ID3D11InputLayout* layout() const;

    //頂点バッファストリームの設定
    void setVertexStream(const VertexStreamDesc* streams, unsigned start = 0);
    //頂点バッファストリームのリセット
    void resetVertexStream();

private:
    //頂点バッファの設定
    //void setVertexBuffer(const Element& element, const VertexStreamDesc& stream, unsigned start);
    //ディスクリプタを変換
    //Element toElement(const InputElementDesc& desc);
    D3D11_INPUT_ELEMENT_DESC toElement(const InputElementDesc& desc);
    //頂点タイプを変換
    const DXGI_FORMAT& toVertexFormat(VertexType type);
    const D3D11_INPUT_CLASSIFICATION toClassification(SlotClass classification);

private:
    //ディスクリプタ配列
    std::vector<InputElementDesc> mDesc;
    //頂点宣言配列
    D3D11_INPUT_ELEMENT_DESC* mElements;
    //コンパイル済みシェーダーコード
    ID3D10Blob* mCompiledShader;
    //頂点レイアウト
    ID3D11InputLayout* mInputLayout;
};

