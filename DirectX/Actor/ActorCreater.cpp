#include "ActorCreater.h"
#include "../Utility/LevelLoader.h"

ActorCreater::ActorCreater() :
    mRenderer(nullptr) {
}

ActorCreater::~ActorCreater() = default;

void ActorCreater::initialize(const std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;
}

std::shared_ptr<Actor> ActorCreater::create(const std::string& type) {
    std::shared_ptr<Actor> actor;
    auto itr = mCreatedActors.find(type);
    if (itr != mCreatedActors.end()) {
        //TODO: コピー対応
        actor = itr->second;
    } else {
        actor = Singleton<LevelLoader>::instance().loadSpecifiedActor(mRenderer, "ActorsList.json", type);
        mCreatedActors.emplace(type, actor);
    }

    return actor;
}
