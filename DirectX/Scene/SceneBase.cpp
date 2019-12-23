#include "SceneBase.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/Game.h"
//#include "../UI/UIManager.h"

SceneBase::SceneBase(std::shared_ptr<Renderer> renderer) :
    mActorManager(new ActorManager()),
    //mUIManager(std::make_unique<UIManager>()),
    mSpriteManager(new SpriteManager()),
    mRenderer(renderer),
    mNext(Scene::NONE) {
    Actor::setActorManager(mActorManager);
    Sprite::setSpriteManager(mSpriteManager);
}

SceneBase::~SceneBase() {
    SAFE_DELETE(mActorManager);
    //mUIManager->clear();
    SAFE_DELETE(mSpriteManager);
}

void SceneBase::update() {
    mActorManager->update();
    updateScene();
    //mUIManager->update();
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
