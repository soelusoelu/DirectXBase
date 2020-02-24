#include "Component.h"
#include "ComponentManager.h"
#include "../Actor/Actor.h"

Component::Component(std::shared_ptr<Actor> owner, const std::string& typeName, int updateOrder) :
    mOwner(owner),
    mUpdateOrder(updateOrder),
    mTypeName(typeName) {
}

Component::~Component() = default;

std::shared_ptr<Actor> Component::owner() const {
    return mOwner.lock();
}

int Component::getUpdateOrder() const {
    return mUpdateOrder;
}

const std::string& Component::getTypeName() const {
    return mTypeName;
}
