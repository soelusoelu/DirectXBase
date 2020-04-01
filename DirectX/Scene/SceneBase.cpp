#include "SceneBase.h"
#include "../Device/Renderer.h"
#include "../System/Game.h"

SceneBase::SceneBase() :
    mRenderer(nullptr),
    mGameObjectManager(nullptr),
    mNext(nullptr) {
}

SceneBase::~SceneBase() = default;

void SceneBase::nextScene(std::shared_ptr<SceneBase> next) {
    mNext = next;
}

void SceneBase::set(std::shared_ptr<Renderer> renderer, GameObjectManager* gameObjectManager) {
    mRenderer = renderer;
    mGameObjectManager = gameObjectManager;
}

std::shared_ptr<SceneBase> SceneBase::getNextScene() const {
    return mNext;
}
