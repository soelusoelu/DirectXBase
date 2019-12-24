#include "SceneManager.h"
#include "GamePlay.h"
#include "Title.h"

SceneManager::SceneManager(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mCurrentScene(std::make_shared<Title>()) {
    setRendererToScene();
}

SceneManager::~SceneManager() = default;

void SceneManager::update() {
    mCurrentScene->update();

    //nullptr‚¶‚á‚È‚¯‚ê‚ÎƒV[ƒ“ˆÚs
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
