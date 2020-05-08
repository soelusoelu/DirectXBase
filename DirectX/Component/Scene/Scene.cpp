#include "Scene.h"

Scene::Scene(const std::shared_ptr<GameObject>& owner) :
    Component(owner, "Scene"),
    mNext("") {
}

Scene::~Scene() = default;

void Scene::next(const std::string& next) {
    mNext = next;
}

const std::string& Scene::getNext() const {
    return mNext;
}
