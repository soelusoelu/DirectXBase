#pragma once

#include "../System/DirectXIncLib.h"
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

class Buffer;
class BufferDesc;
class Camera;
class GBuffer;
class Mesh;
class MeshLoader;
struct PointLight;
class PointLightComponent;
class Shader;
class SoundBase;
class Sound;
class SubResourceDesc;
class Texture;
class VertexStreamDesc;
class ViewportDesc;

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer(const HWND& hWnd);
    ~Renderer();
    void initialize();

    ID3D11Device* device() const;
    ID3D11DeviceContext* deviceContext() const;

    Buffer* createRawBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr) const;
    std::shared_ptr<Buffer> createBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr) const;
    void setViewport(const ViewportDesc& desc);
    void setVertexBuffer(const VertexStreamDesc* stream, unsigned numStream = 1, unsigned start = 0);
    void setIndexBuffer(Buffer* buffer, unsigned offset = 0);
    void setIndexBuffer(std::shared_ptr<Buffer> buffer, unsigned offset = 0);
    void setPrimitive(PrimitiveType primitive) const;
    void setRasterizerStateFront();
    void setRasterizerStateBack();
    void enabledDepthTest();
    void disabledDepthTest();

    std::shared_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<Texture> createTexture(const std::string& fileName, bool isSprite = true);
    std::shared_ptr<Sound> createSound(const std::string& fileName);
    std::shared_ptr<Sound> createSE(const std::string& fileName);
    std::shared_ptr<MeshLoader> createMesh(const std::string& fileName);

    void addPointLight(PointLightComponent* light);
    void removePointLight(PointLightComponent* light);
    void drawPointLights(std::shared_ptr<Camera> camera) const;

    void renderToTexture();
    void renderFromTexture(std::shared_ptr<Camera> camera);

    void draw(unsigned numVertex, unsigned start = 0);
    void drawIndexed(unsigned numIndices, unsigned startIndex = 0, int startVertex = 0);
    void clear(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f, bool depth = true, bool stencil = false);
    void clearRenderTarget(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f);
    void clearDepthStencilView(bool depth = true, bool stencil = false);
    void present();

private:
    void createDeviceAndSwapChain(const HWND& hWnd);
    void createRenderTargetView();
    void createDepthStencilView();
    void createDepthStencilState();
    void createRasterizerState();
    void createBlendState();
    void setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets);
    void setDefaultRenderTarget();
    D3D11_PRIMITIVE_TOPOLOGY toPrimitiveMode(PrimitiveType primitive) const;

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    IDXGISwapChain* mSwapChain;
    ID3D11RenderTargetView* mRenderTargetView;
    ID3D11Texture2D* mDepthStencil;
    ID3D11DepthStencilView* mDepthStencilView;
    ID3D11RasterizerState* mRasterizerState;
    ID3D11RasterizerState* mRasterizerStateBack;
    ID3D11DepthStencilState* mEnableDepthStencilState;
    ID3D11DepthStencilState* mDisableDepthStencilState;
    ID3D11BlendState* mBlendState;

    std::unique_ptr<SoundBase> mSoundBase;
    std::shared_ptr<GBuffer> mGBuffer;
    std::unique_ptr<PointLight> mPointLight;
    Vector3 mAmbientLight;

    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Sound>> mSounds;
    std::unordered_map<std::string, std::shared_ptr<MeshLoader>> mMeshLoaders;
    std::list<PointLightComponent*> mPointLigths;
};
