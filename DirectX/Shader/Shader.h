#pragma once

#include "../System/DirectXIncLib.h"
#include <memory>
#include <string>
#include <vector>

class Buffer;
class Renderer;
class Texture;
class InputElement;
class InputElementDesc;

class Shader {
public:
    Shader(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    ~Shader();
    //シェーダーとのやり取り
    bool map(D3D11_MAPPED_SUBRESOURCE* data, unsigned index = 0, unsigned sub = 0, D3D11_MAP type = D3D11_MAP_WRITE_DISCARD, unsigned flag = 0);
    void unmap(unsigned index = 0, unsigned sub = 0);
    //シェーダーをセット
    void setVertexShader(ID3D11VertexShader* vertex);
    void setPixelShader(ID3D11PixelShader* pixel);
    //自身をシェーダーとして登録
    void setVSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0);
    void setPSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0);
    //コンスタントバッファの作成
    void createConstantBuffer(std::shared_ptr<Renderer> renderer, unsigned bufferSize, unsigned index = 0);
    //使用するコンスタントバッファを登録
    void setVSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1);
    void setPSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1);
    //シェーダの取得
    ID3D11VertexShader* getVertexShader() const;
    ID3D11PixelShader* getPixelShader() const;
    //インプットレイアウトの生成
    void createInputLayout(const InputElementDesc layout[], unsigned numElements);
    //自身を登録
    void setInputLayout();
    //コンスタントバッファの取得
    std::shared_ptr<Buffer> getConstantBuffer(unsigned index = 0) const;

private:
    //シェーダの生成
    void createVertexShader(const std::string& fileName);
    void createPixelShader(const std::string& fileName);

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    ID3D10Blob* mCompileShader;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    std::vector<std::shared_ptr<Buffer>> mConstantBuffers;
    std::shared_ptr<InputElement> mVertexLayout;
};
