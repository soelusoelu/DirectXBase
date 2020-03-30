#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class GameObject;
class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(std::shared_ptr<GameObject> owner);
    ~MeshComponent();
    virtual void onSetActive(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void setMesh(const std::string& fileName);
    const Vector3& getCenter() const;
    float getRadius() const;
    void setActive(bool value);
    bool getActive() const;

private:
    std::shared_ptr<Mesh> mMesh;
};
