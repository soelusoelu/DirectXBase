#include "Field.h"
#include "Transform3D.h"
#include "../Component/MeshComponent.h"
#include "../Device/Time.h"
#include "../Utility/Input.h"

Field::Field(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(renderer, tag) {
    new MeshComponent(this, "test.obj");
}

Field::~Field() = default;

void Field::updateActor() {
    if (Input::getKey(KeyCode::Q)) {
        transform()->rotate(Vector3::up, -60.f * Time::deltaTime);
    }
    if (Input::getKey(KeyCode::W)) {
        transform()->rotate(Vector3::up, 60.f * Time::deltaTime);
    }
    transform()->rotate(Vector3::up, 60.f * Time::deltaTime);
}
