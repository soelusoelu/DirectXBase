#include "SceneManager.h"
#include "GamePlay.h"
#include "Title.h"

SceneManager::SceneManager(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mCurrentScene(std::make_shared<GamePlay>()) {
    setRendererToScene();
}

SceneManager::~SceneManager() = default;

void SceneManager::update() {
    mCurrentScene->update();

    //nullptrじゃなければシーン移行
    auto next = mCurrentScene->getNextScene();
    if (next) {
        mCurrentScene = next;
        setRendererToScene();
    }
}

void SceneManager::draw() const {
    mCurrentScene->draw();
}

void SceneManager::setRendererToScene() {
    mCurrentScene->setRenderer(mRenderer);
}
