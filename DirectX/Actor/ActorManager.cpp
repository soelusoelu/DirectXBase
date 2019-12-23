#include "ActorManager.h"
#include "Actor.h"
#include "PlayerActor.h"
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
}

void ActorManager::add(Actor* add) {
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

std::shared_ptr<PlayerActor> ActorManager::getPlayer() const {
    std::shared_ptr<PlayerActor> p = nullptr;
    for (const auto& actor : mActors) {
        p = std::dynamic_pointer_cast<PlayerActor>(actor);
        if (p) {
            return p;
        }
    }
    //ÅŒã‚Ü‚ÅŒ©‚Â‚©‚ç‚È‚¯‚ê‚Înullptr‚ð•Ô‚·
    return p;
}

void ActorManager::remove() {
    auto itr = mActors.begin();
    while (itr != mActors.end()) {
        if ((*itr)->getState() == ActorState::DEAD) {
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
