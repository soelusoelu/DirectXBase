#pragma once

#include "../System/BufferDesc.h"
#include "../System/DirectXIncLib.h"
#include "../System/SamplerDesc.h"
#include "../System/SubResourceDesc.h"
#include "../System/Texture2DDesc.h"
#include "../System/ViewportDesc.h"
#include "../Utility/Math.h"
#include <list>
#include <memory>
#include <string>
#include <unordered_map>

enum class PrimitiveType {
    PRIMITIVE_TYPE_POINT_LIST,
    PRIMITIVE_TYPE_LINE_LIST,
    PRIMITIVE_TYPE_LINE_STRIP,
    PRIMITIVE_TYPE_TRIANGLE_LIST,
    PRIMITIVE_TYPE_TRIANGLE_STRIP
};

class BlendState;
class Buffer;
class Camera;
class DepthStencilState;
class GBuffer;
class IndexBuffer;
class Mesh;
class MeshLoader;
struct PointLight;
class PointLightComponent;
class RasterizerState;
class Sampler;
class Shader;
class SoundBase;
class Sound;
class Texture;
class Texture2D;
class VertexBuffer;

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer(const HWND& hWnd);
    ~Renderer();
    void initialize();

    ID3D11Device* device() const;
    ID3D11DeviceContext* deviceContext() const;
    std::shared_ptr<BlendState> blendState() const;
    std::shared_ptr<DepthStencilState> depthStencilState() const;
    std::shared_ptr<RasterizerState> rasterizerState() const;

    std::shared_ptr<Buffer> createBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr);
    std::shared_ptr<VertexBuffer> createVertexBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr);
    std::shared_ptr<IndexBuffer> createIndexBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr);
    std::shared_ptr<Texture2D> createTexture2D(const Texture2DDesc& desc, const SubResourceDesc* data = nullptr) const;
    std::shared_ptr<Sampler> createSamplerState(const SamplerDesc& desc);
    void setViewport(const ViewportDesc& desc);
    void setPrimitive(PrimitiveType primitive) const;

    std::shared_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<Texture> createTexture(const std::string& fileName, bool isSprite = true);
    std::shared_ptr<Sound> createSound(const std::string& fileName);
    std::shared_ptr<Sound> createSE(const std::string& fileName);
    std::shared_ptr<MeshLoader> createMesh(const std::string& fileName);

    void addPointLight(PointLightComponent* light);
    void removePointLight(PointLightComponent* light);
    void drawPointLights(std::shared_ptr<Camera> camera);

    void renderToTexture();
    void renderFromTexture(std::shared_ptr<Camera> camera);

    void draw(unsigned numVertex, unsigned start = 0);
    void drawIndexed(unsigned numIndices, unsigned startIndex = 0, int startVertex = 0);
    void clearRenderTarget(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f);
    void clearDepthStencilView(bool depth = true, bool stencil = false);
    void present();

private:
    void createDeviceAndSwapChain(const HWND& hWnd);
    void createRenderTargetView();
    void createDepthStencilView();
    void setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets);
    D3D11_PRIMITIVE_TOPOLOGY toPrimitiveMode(PrimitiveType primitive) const;

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    IDXGISwapChain* mSwapChain;
    ID3D11RenderTargetView* mRenderTargetView;
    ID3D11DepthStencilView* mDepthStencilView;

    std::unique_ptr<SoundBase> mSoundBase;
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthStencilState;
    std::shared_ptr<RasterizerState> mRasterizerState;
    std::shared_ptr<GBuffer> mGBuffer;
    std::shared_ptr<PointLight> mPointLight;
    Vector3 mAmbientLight;

    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Sound>> mSounds;
    std::unordered_map<std::string, std::shared_ptr<MeshLoader>> mMeshLoaders;
    std::list<PointLightComponent*> mPointLigths;
};
