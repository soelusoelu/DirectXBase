#include "SceneManager.h"
#include "GamePlay.h"

SceneManager::SceneManager(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mCurrentScene(std::make_shared<GamePlay>(renderer)) {
}

SceneManager::~SceneManager() = default;

void SceneManager::update() {
    mCurrentScene->update();

    if (mCurrentScene->getNextScene() != Scene::NONE) {
        change(mCurrentScene->getNextScene());
    }
}

void SceneManager::draw() const {
    mCurrentScene->draw();
}

void SceneManager::change(Scene next) {
    mCurrentScene.reset();
    if (next == Scene::GAME_PLAY) {
        mCurrentScene = std::make_shared<GamePlay>(mRenderer);
    }
}
