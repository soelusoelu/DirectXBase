#include "ActorManager.h"
#include "Actor.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Hierarchy.h"
#include <algorithm>
#include <iterator>

ActorManager::ActorManager() :
    mUpdatingActors(false) {
}

ActorManager::~ActorManager() = default;

void ActorManager::update() {
    mUpdatingActors = true;
    for (auto&& actor : mActors) {
        actor->update();
    }
    mUpdatingActors = false;

    movePendingToMain();

    remove();

    Debug::hierarchy()->update(mActors);
}

void ActorManager::add(ActorPtr add) {
    if (mUpdatingActors) {
        mPendingActors.emplace_back(add);
    } else {
        mActors.emplace_back(add);
    }
}

void ActorManager::clear() {
    mPendingActors.clear();
    mActors.clear();
}

std::shared_ptr<Actor> ActorManager::getPlayer() const {
    for (const auto& actor : mActors) {
        //タグでプレイヤーか判断
        if (actor->tag() == "Player") {
            return actor;
        }
    }
    //最後まで見つからなければnullptrを返す
    return nullptr;
}

void ActorManager::remove() {
    auto itr = mActors.begin();
    while (itr != mActors.end()) {
        if ((*itr)->isDead()) {
            itr = mActors.erase(itr);
        } else {
            ++itr;
        }
    }
}

void ActorManager::movePendingToMain() {
    if (mPendingActors.empty()) {
        return;
    }
    std::copy(mPendingActors.begin(), mPendingActors.end(), std::back_inserter(mActors));
    mPendingActors.clear();
}
