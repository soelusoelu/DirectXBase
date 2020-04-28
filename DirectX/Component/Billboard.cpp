#include "Billboard.h"

Billboard::Billboard(std::shared_ptr<GameObject> owner) :
    Sprite3D(owner, "Billboard") {
}

Billboard::~Billboard() = default;
