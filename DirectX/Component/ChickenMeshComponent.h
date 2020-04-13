#pragma once

#include "MeshComponent.h"
#include "../System/Game.h"

struct ChickenMeshConstantBuffer {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 wvp; //ワールドから射影までの変換行列
    ALIGN16 Vector3 pos;
    ALIGN16 Vector3 upColor;
    ALIGN16 Vector3 bottomColor;
};

class ChickenMeshComponent : public MeshComponent {
public:
    ChickenMeshComponent(std::shared_ptr<GameObject> owner);
    ~ChickenMeshComponent();
    virtual void setMesh(const std::string& fileName) override;
    virtual void setShader() override;
    virtual void draw() override;
    void setUpColor(const Vector3& color);
    void setBottomColor(const Vector3& color);

private:
    Vector3 mUpColor;
    Vector3 mBottomColor;
};
