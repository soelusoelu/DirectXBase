#include "Scene.h"

Scene::Scene() :
    Component(),
    mNext("") {
}

Scene::~Scene() = default;

void Scene::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    for (const auto& tag : mTagsToNext) {
        info.first = "ExclusionList";
        info.second = tag;
        inspect->emplace_back(info);
    }
}

void Scene::next(const std::string& next) {
    mNext = next;
}

const std::string& Scene::getNext() const {
    return mNext;
}

void Scene::addObjectToNext(const std::string& tag) {
    mTagsToNext.emplace(tag);
}

std::unordered_set<std::string> Scene::getObjectToNext() const {
    return mTagsToNext;
}
