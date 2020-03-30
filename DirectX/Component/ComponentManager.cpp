#include "ComponentManager.h"
#include "Component.h"

ComponentManager::ComponentManager(std::shared_ptr<GameObject> owner) :
    mOwner(owner) {
}

ComponentManager::~ComponentManager() {
    mStartComponents.clear();
    mComponents.clear();
}

void ComponentManager::start() {
    if (mStartComponents.empty()) {
        return;
    }

    for (auto&& comp : mStartComponents) {
        comp->start();

        int myOrder = comp->getUpdateOrder();
        auto itr = mComponents.begin();
        for (; itr != mComponents.end(); ++itr) {
            if (myOrder < (*itr)->getUpdateOrder()) {
                break;
            }
        }
        mComponents.insert(itr, comp);
    }
    mStartComponents.clear();
}

void ComponentManager::update() {
    for (auto&& comp : mComponents) {
        comp->update();
    }
}

void ComponentManager::addComponent(ComponentPtr component) {
    mStartComponents.emplace_back(component);
}

void ComponentManager::onUpdateWorldTransform() {
    for (auto&& comp : mComponents) {
        comp->onUpdateWorldTransform();
    }
}

void ComponentManager::onSetActive(bool value) {
    for (auto&& comp : mComponents) {
        comp->onSetActive(value);
    }
}

std::list<std::shared_ptr<Component>> ComponentManager::getAllComponents() const {
    return mComponents;
}
