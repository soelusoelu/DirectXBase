#include "Field.h"
#include "Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Device/Time.h"
#include "../Utility/Input.h"

Field::Field(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(renderer, tag) {
}

Field::~Field() = default;

void Field::start() {
    auto mesh = std::make_shared<MeshComponent>(shared_from_this());
    mesh->setMesh("test.obj");
    mComponentManager->addComponent(mesh);
}

void Field::updateActor() {
    if (Input::getKey(KeyCode::Q)) {
        transform()->rotate(Vector3::up, -60.f * Time::deltaTime);
    }
    if (Input::getKey(KeyCode::E)) {
        transform()->rotate(Vector3::up, 60.f * Time::deltaTime);
    }
    //transform()->rotate(Vector3::up, 60.f * Time::deltaTime);
}
