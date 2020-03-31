#pragma once

#include "MeshComponent.h"
#include "../Math/Math.h"
#include <string>

class Mesh;

class TransparentMeshComponent : public MeshComponent {
public:
    TransparentMeshComponent(std::shared_ptr<GameObject> owner);
    ~TransparentMeshComponent();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    virtual void setMesh(const std::string& fileName) override;
    void setAlpha(float alpha);
    float getAlpha() const;
};
