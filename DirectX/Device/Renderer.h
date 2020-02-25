﻿#pragma once

#include "../System/DirectXIncLib.h"
#include "../Utility/Math.h"
#include <list>
#include <memory>

class AssetsManager;
class Camera;
class DirectionalLight;
class GBuffer;
struct PointLight;
class PointLightComponent;

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer();
    ~Renderer();
    void initialize();
    void update();

    std::shared_ptr<AssetsManager> getAssetsManager() const;

    const Vector3& getAmbientLight() const;
    void setAmbientLight(const Vector3& ambient);
    std::shared_ptr<DirectionalLight> getDirectionalLight() const;
    void addPointLight(std::shared_ptr<PointLightComponent> light);
    void drawPointLights(std::shared_ptr<Camera> camera);

    void renderToTexture();
    void renderFromTexture(std::shared_ptr<Camera> camera);

private:
    void removePointLight();

private:
    std::shared_ptr<AssetsManager> mAssetsManager;
    std::unique_ptr<GBuffer> mGBuffer;

    Vector3 mAmbientLight;
    std::shared_ptr<DirectionalLight> mDirectionalLight;
    std::shared_ptr<PointLight> mPointLight;

    std::list<std::weak_ptr<PointLightComponent>> mPointLights;
};
