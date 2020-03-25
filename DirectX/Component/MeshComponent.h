#pragma once

#include "Component.h"
#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class Actor;
class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(std::shared_ptr<Actor> owner);
    ~MeshComponent();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void setMesh(const std::string& fileName);
    const Vector3& getCenter() const;
    float getRadius() const;
    void setActive(bool value);
    bool getActive() const;

private:
    std::shared_ptr<Mesh> mMesh;
};
