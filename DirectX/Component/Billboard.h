#pragma once

#include "Sprite3D.h"

class Billboard : public Sprite3D {
public:
    Billboard(std::shared_ptr<GameObject> owner);
    ~Billboard();
};
