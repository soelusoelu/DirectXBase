#pragma once

#include "MeshComponent.h"

class ChickenMeshComponent : public MeshComponent {
public:
    ChickenMeshComponent(std::shared_ptr<GameObject> owner);
    ~ChickenMeshComponent();
};
