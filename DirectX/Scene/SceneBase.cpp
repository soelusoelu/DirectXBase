#include "SceneBase.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/Game.h"
#include "../UI/UI.h"
#include "../UI/UIManager.h"

SceneBase::SceneBase() :
    mUIManager(new UIManager()),
    mSpriteManager(new SpriteManager()),
    mRenderer(nullptr),
    mNext(nullptr),
    mIsStarted(false) {
    UI::setUIManager(mUIManager);
    Sprite::setSpriteManager(mSpriteManager);
}

SceneBase::~SceneBase() {
    SAFE_DELETE(mUIManager);
    SAFE_DELETE(mSpriteManager);
}

void SceneBase::update() {
    start();
    updateScene();
    mUIManager->update();
    mSpriteManager->update();
}

void SceneBase::draw() {
    if (!mRenderer) {
        return;
    }
    mSpriteManager->draw(mRenderer);
}

void SceneBase::next(std::shared_ptr<SceneBase> next) {
    mNext = next;
}

std::shared_ptr<SceneBase> SceneBase::getNextScene() const {
    return mNext;
}

void SceneBase::setRenderer(std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;
}

void SceneBase::start() {
    if (!mIsStarted) {
        startScene();
        mIsStarted = true;
    }
}
