#include "InputElement.h"
#include "DirectXIncLib.h"
#include "Game.h"

InputElement::InputElement(ID3D11Device* device, const InputElementDesc* desc, unsigned numElements, ID3D10Blob* compile) :
    mDesc(&desc[0], &desc[numElements]),
    mElements(new D3D11_INPUT_ELEMENT_DESC[numElements]),
    mCompiledShader(compile) {
    for (unsigned i = 0; i < numElements; ++i) {
        mElements[i] = toElement(mDesc[i]);
    }

    //頂点インプットレイアウトを作成
    device->CreateInputLayout(mElements, numElements, mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), &mInputLayout);
}

InputElement::~InputElement() {
    delete[] mElements;
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

void InputElement::setVertexStream(const VertexStreamDesc* streams, unsigned start) {
    //頂点ストリームのリセット
    //resetVertexStream();
    //頂点バッファを設定
    //for (const auto& e : mElements) {
    //    setVertexBuffer(e, streams[e.stream], start);
    //}
}

void InputElement::resetVertexStream() {
    //頂点配列を無効にする
    //for (unsigned attr = 0; attr < VERTEX_DECLARATION_MAX; ++attr) {
    //    glDisableVertexAttribArray(attr);
    //}
}

//void InputElement::setVertexBuffer(const Element& element, const VertexStreamDesc& stream, unsigned start) {
//    D3D11_INPUT_ELEMENT_DESC layout[] = {
//        element.attrib,
//        element.index,
//        element.format,
//        element.slot,
//        element.offset,
//        element.classification,
//        element.stream
//    };
//    if (FAILED(Direct3D11::mDevice->CreateInputLayout(layout, 1, mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), &mVertexLayout))) {
//        SAFE_RELEASE(mCompiledShader);
//        return;
//    }
//}

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
