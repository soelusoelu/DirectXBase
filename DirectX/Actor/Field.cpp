#include "Field.h"
#include "../Component/MeshComponent.h"

Field::Field(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(renderer, tag) {
    new MeshComponent(this, "sky_and_ground.obj");
}

Field::~Field() = default;

void Field::updateActor() {
}
