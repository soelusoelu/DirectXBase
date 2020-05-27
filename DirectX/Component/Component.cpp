#include "Component.h"

Component::Component(int updateOrder) :
    mOwner(),
    mUpdateOrder(updateOrder),
    mComponentName("") {
}

Component::~Component() = default;

std::shared_ptr<GameObject> Component::owner() const {
    return mOwner.lock();
}

int Component::getUpdateOrder() const {
    return mUpdateOrder;
}

const std::string& Component::getComponentName() const {
    return mComponentName;
}
