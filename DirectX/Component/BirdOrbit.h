#pragma once

#include "Component.h"

class BirdOrbit : public Component {
public:
    BirdOrbit(std::shared_ptr<GameObject> owner);
    ~BirdOrbit();
};
