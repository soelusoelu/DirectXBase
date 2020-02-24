#pragma once

#include "Component.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class Actor;
class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(std::shared_ptr<Actor> owner);
    ~MeshComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void setMesh(const std::string& fileName);
    std::shared_ptr<Mesh> getMesh() const;
    void setActive(bool value);
    bool getActive() const;

private:
    std::shared_ptr<Mesh> mMesh;
};
