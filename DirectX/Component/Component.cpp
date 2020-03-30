#include "Component.h"
#include "ComponentManager.h"

Component::Component(GameObjectPtr owner, const std::string& type, int updateOrder) :
    mOwner(owner),
    mUpdateOrder(updateOrder),
    mType(type) {
}

Component::~Component() = default;

std::shared_ptr<GameObject> Component::owner() const {
    return mOwner.lock();
}

int Component::getUpdateOrder() const {
    return mUpdateOrder;
}

const std::string& Component::getTypeName() const {
    return mType;
}
