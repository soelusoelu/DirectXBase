#include "Component.h"
#include "ComponentManager.h"
#include "../Actor/Actor.h"

Component::Component(Actor* owner, int updateOrder) :
    mOwner(owner),
    mUpdateOrder(updateOrder) {
    mOwner->componentManager()->addComponent(this);
}

Component::~Component() = default;

int Component::getUpdateOrder() const {
    return mUpdateOrder;
}

Actor* Component::getOwner() const {
    return mOwner;
}
