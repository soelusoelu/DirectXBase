#include "Component.h"

Component::Component() :
    mOwner(),
    mComponentName("") {
}

Component::~Component() = default;

std::shared_ptr<GameObject> Component::owner() const {
    return mOwner.lock();
}

const std::string& Component::getComponentName() const {
    return mComponentName;
}
