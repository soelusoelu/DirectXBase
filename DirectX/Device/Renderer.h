#pragma once

#include "../System/DirectXIncLib.h"
#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>

class AssetsManager;
class Camera;
class DirectionalLight;
class DrawString;
class GBuffer;
struct PointLight;
class PointLightComponent;

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer();
    ~Renderer();
    void initialize();
    void update();
    void loadProperties(const rapidjson::Value& inObj);

    std::shared_ptr<AssetsManager> getAssetsManager() const;
    std::shared_ptr<DrawString> getDrawString() const;

    const Vector3& getAmbientLight() const;
    void setAmbientLight(const Vector3& ambient);
    std::shared_ptr<DirectionalLight> getDirectionalLight() const;
    void addPointLight(std::shared_ptr<PointLightComponent> light);
    void drawPointLights(std::shared_ptr<Camera> camera);

    void renderToTexture();
    void renderFromTexture(std::shared_ptr<Camera> camera);
    void renderSprite(Matrix4* proj);
    void renderToDebug(Matrix4* proj);

private:
    void removePointLight();

private:
    std::shared_ptr<AssetsManager> mAssetsManager;
    std::shared_ptr<DrawString> mDrawString;
    std::unique_ptr<GBuffer> mGBuffer;

    Vector3 mAmbientLight;
    std::shared_ptr<DirectionalLight> mDirectionalLight;
    std::shared_ptr<PointLight> mPointLight;

    std::list<std::weak_ptr<PointLightComponent>> mPointLights;
};
