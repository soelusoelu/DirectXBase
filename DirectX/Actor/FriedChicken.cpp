#include "FriedChicken.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Log.h"

FriedChicken::FriedChicken(std::shared_ptr<Renderer> renderer) :
    Actor(renderer, "FriedChicken") {
}

FriedChicken::~FriedChicken() = default;

void FriedChicken::start() {
    Debug::log()->log("FriedChicken spawn.");
}

void FriedChicken::updateActor() {
}
