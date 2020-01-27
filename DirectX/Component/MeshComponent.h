#pragma once

#include "Component.h"
#include <memory>

class Actor;
class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(Actor* owner, const char* filename);
    ~MeshComponent();
    virtual void start() override;
    virtual void update() override;
    std::shared_ptr<Mesh> getMesh() const;
    void setActive(bool value);
    bool getActive() const;

private:
    std::shared_ptr<Mesh> mMesh;
};
