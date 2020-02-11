#pragma once

#include "Component.h"
#include <memory>
#include <string>

class Actor;
class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(Actor* owner, const std::string& filename);
    ~MeshComponent();
    virtual void start() override;
    virtual void update() override;
    Mesh* getMesh() const;
    void setActive(bool value);
    bool getActive() const;

private:
    Mesh* mMesh;
};
