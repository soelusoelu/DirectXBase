#include "SceneBase.h"
#include "../Device/Renderer.h"
#include "../System/Game.h"

SceneBase::SceneBase() :
    mGameObjectManager(nullptr),
    mNext(nullptr) {
}

SceneBase::~SceneBase() = default;

void SceneBase::nextScene(std::shared_ptr<SceneBase> next) {
    mNext = next;
}

void SceneBase::set(GameObjectManager* gameObjectManager) {
    mGameObjectManager = gameObjectManager;
}

std::shared_ptr<SceneBase> SceneBase::getNextScene() const {
    return mNext;
}
