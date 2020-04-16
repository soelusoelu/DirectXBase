#pragma once

#include "MeshComponent.h"
#include "../System/Game.h"

struct ChickenMeshConstantBuffer {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 wvp; //ワールドから射影までの変換行列
};

class ChickenMeshComponent : public MeshComponent {
public:
    ChickenMeshComponent(std::shared_ptr<GameObject> owner);
    ~ChickenMeshComponent();
    virtual void setMesh(const std::string& fileName) override;
    virtual void setShader() override;
    virtual void draw() override;
};
