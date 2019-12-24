#pragma once

#include "../System/DirectXIncLib.h"
#include <memory>

class Buffer;
class Renderer;
class Texture;

class Shader {
public:
    Shader(std::shared_ptr<Renderer> renderer, const char* fileName, const char* VSFuncName, const char* PSFuncName);
    ~Shader();
    //シェーダーをセット
    void setVertexShader(ID3D11VertexShader* vertex);
    void setPixelShader(ID3D11PixelShader* pixel);
    //自身をシェーダーとして登録
    void setVSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0);
    void setPSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0);
    //使用するコンスタントバッファを登録
    void setVSConstantBuffers(unsigned start = 0, unsigned numBuffers = 1);
    void setPSConstantBuffers(unsigned start = 0, unsigned numBuffers = 1);
    //テクスチャの登録
    void setVSTextures(std::shared_ptr<Texture> texture, unsigned start = 0, unsigned numTextures = 1);
    void setPSTextures(std::shared_ptr<Texture> texture, unsigned start = 0, unsigned numTextures = 1);
    //シェーダの取得
    ID3D11VertexShader* getVertexShader() const;
    ID3D11PixelShader* getPixelShader() const;
    //コンパイル済みシェーダの取得
    ID3D10Blob* getCompiledShader() const;
    //コンスタントバッファの取得
    std::shared_ptr<Buffer> getConstantBuffer() const;

private:
    //シェーダの生成
    void createVertexShader(const char* fileName, const char* funcName);
    void createPixelShader(const char* fileName, const char* funcName);

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    ID3D10Blob* mCompileShader;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    std::shared_ptr<Buffer> mConstantBuffer;
};
