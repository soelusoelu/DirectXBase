#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>

class Camera;
class DirectionalLight;
class DrawString;
class GBuffer;
struct PointLight;
class PointLightComponent;

class Renderer {
public:
    Renderer();
    ~Renderer();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();

    const std::shared_ptr<DrawString>& getDrawString() const;

    const Vector3& getAmbientLight() const;
    void setAmbientLight(const Vector3& ambient);
    void addPointLight(const std::shared_ptr<PointLightComponent>& light);
    void drawPointLights();

    void renderToTexture();
    void renderFromTexture(const Camera& camera, const DirectionalLight& dirLight);
    //スプライト描画共通処理
    void renderSprite() const;
    //2Dスプライト処理
    void renderSprite2D(Matrix4* proj) const;
    //3Dスプライト処理
    void renderSprite3D() const;
    //デバッグ画面処理
    void renderToDebug(Matrix4* proj) const;

private:
    void removePointLight();

private:
    std::shared_ptr<DrawString> mDrawString;
    std::unique_ptr<GBuffer> mGBuffer;

    Vector3 mAmbientLight;
    std::shared_ptr<PointLight> mPointLight;

    std::list<std::weak_ptr<PointLightComponent>> mPointLights;
};
