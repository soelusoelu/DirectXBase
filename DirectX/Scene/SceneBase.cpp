#include "SceneBase.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/Game.h"
#include "../UI/UI.h"
#include "../UI/UIManager.h"

SceneBase::SceneBase(std::shared_ptr<Renderer> renderer) :
    mActorManager(new ActorManager()),
    mUIManager(new UIManager()),
    mSpriteManager(new SpriteManager()),
    mRenderer(renderer),
    mNext(Scene::NONE) {
    Actor::setActorManager(mActorManager);
    UI::setUIManager(mUIManager);
    Sprite::setSpriteManager(mSpriteManager);
}

SceneBase::~SceneBase() {
    SAFE_DELETE(mActorManager);
    SAFE_DELETE(mUIManager);
    SAFE_DELETE(mSpriteManager);
}

void SceneBase::update() {
    mActorManager->update();
    updateScene();
    mUIManager->update();
    mSpriteManager->update();
}

void SceneBase::draw() {
    mSpriteManager->draw(mRenderer);
}

void SceneBase::next(Scene next) {
    mNext = next;
}

Scene SceneBase::getNextScene() const {
    return mNext;
}
