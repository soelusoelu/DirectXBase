#include "GameObjectManager.h"
#include "GameObject.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/Hierarchy.h"
#include "../Utility/LevelLoader.h"
#include <algorithm>
#include <iterator>

GameObjectManager::GameObjectManager() :
    mUpdatingGameObjects(false) {
}

GameObjectManager::~GameObjectManager() = default;

void GameObjectManager::update() {
    mUpdatingGameObjects = true;
    for (auto&& gameObject : mGameObjects) {
        gameObject->update();
    }
    mUpdatingGameObjects = false;

    movePendingToMain();

    remove();

    DebugUtility::hierarchy()->setGameObjectToButton(mGameObjects);
}

void GameObjectManager::add(GameObjectPtr add) {
    if (mUpdatingGameObjects) {
        mPendingGameObjects.emplace_back(add);
    } else {
        mGameObjects.emplace_back(add);
    }
}

void GameObjectManager::clear() {
    for (auto&& gameObject : mGameObjects) {
        gameObject->finalize();
    }
    for (auto&& gameObject : mPendingGameObjects) {
        gameObject->finalize();
    }

    auto itr = mGameObjects.begin();
    while (itr != mGameObjects.end()) {
        if ((*itr)->tag() != "Camera" && (*itr)->tag() != "DirectionalLight") {
            itr = mGameObjects.erase(itr);
        } else {
            ++itr;
        }
    }
    mPendingGameObjects.clear();
}

std::shared_ptr<GameObject> GameObjectManager::find(const std::string& tag) const {
    for (const auto& gameObject : mGameObjects) {
        //タグでプレイヤーか判断
        if (gameObject->tag() == tag) {
            return gameObject;
        }
    }
    for (const auto& gameObject : mPendingGameObjects) {
        //タグでプレイヤーか判断
        if (gameObject->tag() == tag) {
            return gameObject;
        }
    }
    //最後まで見つからなければnullptrを返す
    return nullptr;
}

std::shared_ptr<GameObject> GameObjectManager::getPlayer() const {
    return find("Player");
}

void GameObjectManager::remove() {
    auto itr = mGameObjects.begin();
    while (itr != mGameObjects.end()) {
        if ((*itr)->isDead()) {
            (*itr)->finalize();
            itr = mGameObjects.erase(itr);
        } else {
            ++itr;
        }
    }
}

void GameObjectManager::movePendingToMain() {
    if (mPendingGameObjects.empty()) {
        return;
    }
    std::copy(mPendingGameObjects.begin(), mPendingGameObjects.end(), std::back_inserter(mGameObjects));
    mPendingGameObjects.clear();
}
