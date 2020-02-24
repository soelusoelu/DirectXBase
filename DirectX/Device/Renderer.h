#pragma once

#include "../System/DirectXIncLib.h"
#include "../Utility/Math.h"
#include <list>
#include <memory>

class AssetsManager;
class Camera;
class GBuffer;
struct PointLight;
class PointLightComponent;

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer();
    ~Renderer();
    void initialize();

    std::shared_ptr<AssetsManager> getAssetsManager() const;

    void setAmbientLight(const Vector3& ambient);
    void addPointLight(PointLightComponent* light);
    void removePointLight(PointLightComponent* light);
    void drawPointLights(std::shared_ptr<Camera> camera);

    void renderToTexture();
    void renderFromTexture(std::shared_ptr<Camera> camera);

private:
    std::shared_ptr<AssetsManager> mAssetsManager;
    std::unique_ptr<GBuffer> mGBuffer;

    Vector3 mAmbientLight;
    std::shared_ptr<PointLight> mPointLight;

    std::list<PointLightComponent*> mPointLigths;
};
