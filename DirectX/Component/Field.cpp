#include "Field.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Math/Math.h"

Field::Field(std::shared_ptr<GameObject> owner) :
    Component(owner, "Field") {
}

Field::~Field() = default;

void Field::start() {
    auto t = owner()->transform();
    t->rotate(Vector3::left, 90.f);
    t->setPosition(Vector3(5.f, 0.f, 5.f));
}

void Field::update() {
    //owner()->transform()->rotate(Vector3::up, 0.5f);
}
