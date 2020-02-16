#pragma once

#include "DirectXIncLib.h"
#include "InputElementDesc.h"
#include <vector>

class InputElement {
public:
    InputElement(ID3D11Device* device, const InputElementDesc desc[], unsigned numElements, ID3D10Blob* compile);
    ~InputElement();
    //ディスクリプタの要素数を返す
    size_t size() const;
    //ディスクリプタの取得
    const InputElementDesc& desc(unsigned index) const;
    //頂点レイアウトの取得
    ID3D11InputLayout* layout() const;

private:
    //ディスクリプタを変換
    D3D11_INPUT_ELEMENT_DESC toElement(const InputElementDesc& desc);
    //頂点タイプを変換
    const DXGI_FORMAT& toVertexFormat(VertexType type);
    const D3D11_INPUT_CLASSIFICATION toClassification(SlotClass classification);

private:
    //ディスクリプタ配列
    std::vector<InputElementDesc> mDesc;
    //頂点レイアウト
    ID3D11InputLayout* mInputLayout;
};

