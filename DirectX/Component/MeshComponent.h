#pragma once

#include "Component.h"
#include <memory>

class Actor;
class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(Actor* owner, const char* filename);
    virtual void start() override;
    virtual void update() override;
    std::shared_ptr<Mesh> getMesh() const;

private:
    std::shared_ptr<Mesh> mMesh;
};
