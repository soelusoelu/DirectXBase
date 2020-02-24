#include "Component.h"
#include "ComponentManager.h"
#include "../Actor/Actor.h"

Component::Component(std::shared_ptr<Actor> owner, int updateOrder) :
    mOwner(owner),
    mUpdateOrder(updateOrder) {
}

Component::~Component() = default;

int Component::getUpdateOrder() const {
    return mUpdateOrder;
}

std::shared_ptr<Actor> Component::owner() const {
    return mOwner.lock();
}
