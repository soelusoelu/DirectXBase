#include "InputElement.h"
#include "DirectX.h"
#include "Game.h"

InputElement::InputElement(const InputElementDesc desc[], unsigned numElements, ID3D10Blob* compile) :
    mDesc(&desc[0], &desc[numElements]) {
    auto mElements = new D3D11_INPUT_ELEMENT_DESC[numElements];
    for (unsigned i = 0; i < numElements; ++i) {
        mElements[i] = toElement(mDesc[i]);
    }

    //頂点インプットレイアウトを作成
    Singleton<DirectX>::instance().device()->CreateInputLayout(mElements, numElements, compile->GetBufferPointer(), compile->GetBufferSize(), &mInputLayout);

    SAFE_DELETE_ARRAY(mElements);
}

InputElement::~InputElement() {
    SAFE_RELEASE(mInputLayout);
}

size_t InputElement::size() const {
    return mDesc.size();
}

const InputElementDesc& InputElement::desc(unsigned index) const {
    return mDesc[index];
}

ID3D11InputLayout* InputElement::layout() const {
    return mInputLayout;
}

D3D11_INPUT_ELEMENT_DESC InputElement::toElement(const InputElementDesc& desc) {
    D3D11_INPUT_ELEMENT_DESC element = {
        desc.semantic,
        desc.index,
        toVertexFormat(desc.type),
        desc.slot,
        desc.offset,
        toClassification(desc.classification),
        desc.stream
    };
    return element;
}

const DXGI_FORMAT& InputElement::toVertexFormat(VertexType type) {
    const DXGI_FORMAT vertexFormats[] = {
        DXGI_FORMAT_R32_FLOAT,
        DXGI_FORMAT_R32G32_FLOAT,
        DXGI_FORMAT_R32G32B32_FLOAT,
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_R32_UINT,
        DXGI_FORMAT_R32G32_UINT,
        DXGI_FORMAT_R32G32B32_UINT,
        DXGI_FORMAT_R32G32B32A32_UINT,
    };
    return vertexFormats[static_cast<int>(type)];
}

const D3D11_INPUT_CLASSIFICATION InputElement::toClassification(SlotClass classification) {
    const D3D11_INPUT_CLASSIFICATION classifications[]{
        D3D11_INPUT_PER_VERTEX_DATA,
        D3D11_INPUT_PER_INSTANCE_DATA
    };
    return classifications[static_cast<int>(classification)];
}
