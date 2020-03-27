#include "Field.h"
#include "Transform3D.h"
#include "../Device/Time.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"

Field::Field(std::shared_ptr<Renderer> renderer) :
    Actor(renderer, "Field") {
}

Field::~Field() = default;

void Field::start() {
    mTransform->setRotation(Vector3::up, 90.f);
}

void Field::updateActor() {
    if (Input::keyboard()->getKey(KeyCode::Q)) {
        mTransform->rotate(Vector3::up, -60.f * Time::deltaTime);
    }
    if (Input::keyboard()->getKey(KeyCode::E)) {
        mTransform->rotate(Vector3::up, 60.f * Time::deltaTime);
    }
}
