#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <string>

class Mesh;

class TransparentMeshComponent : public Component {
public:
    TransparentMeshComponent(std::shared_ptr<GameObject> owner);
    ~TransparentMeshComponent();
    virtual void onSetActive(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    void setMesh(const std::string& fileName);
    const Vector3& getCenter() const;
    float getRadius() const;
    void setAlpha(float alpha);
    float getAlpha() const;
    void setActive(bool value);
    bool getActive() const;

private:
    std::shared_ptr<Mesh> mMesh;
};
