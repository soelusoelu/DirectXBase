#include "EmptyActor.h"

EmptyActor::EmptyActor(std::shared_ptr<Renderer> renderer) :
    Actor(renderer, "Empty") {
}

EmptyActor::~EmptyActor() = default;

void EmptyActor::start() {
}

void EmptyActor::updateActor() {
}
