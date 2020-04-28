#include "BirdOrbit.h"

BirdOrbit::BirdOrbit(std::shared_ptr<GameObject> owner) :
    Component(owner, "BirdOrbit")
{
}

BirdOrbit::~BirdOrbit() = default;
