#pragma once

#include "MeshComponent.h"
#include "../Math/Math.h"
#include <string>

struct TransparentConstantBuffer {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 WVP; //ワールドから射影までの変換行列
    ALIGN16 Vector3 lightDir; //ワールド行列
    ALIGN16 Vector3 cameraPos; //ワールドから射影までの変換行列
};

class DirectionalLight;

class TransparentMeshComponent : public MeshComponent {
public:
    TransparentMeshComponent();
    ~TransparentMeshComponent();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    virtual void setMesh(const std::string& fileName) override;
    virtual void setShader() override;
    virtual void draw() override;
    void setAlpha(float alpha);
    float getAlpha() const;

private:
    std::shared_ptr<DirectionalLight> mDirLight;
    float mAlpha;
};
