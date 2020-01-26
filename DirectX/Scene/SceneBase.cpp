#include "SceneBase.h"
#include "../Camera/Camera.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/Game.h"
#include "../UI/UI.h"
#include "../UI/UIManager.h"

SceneBase::SceneBase() :
    mRenderer(nullptr),
    mMeshManager(new MeshManager()),
    mSpriteManager(new SpriteManager()),
    mUIManager(new UIManager()),
    mCamera(std::make_shared<Camera>()),
    mNext(nullptr),
    mIsStarted(false) {
    Mesh::setMeshManager(mMeshManager);
    Sprite::setSpriteManager(mSpriteManager);
    UI::setUIManager(mUIManager);
}

SceneBase::~SceneBase() {
    SAFE_DELETE(mSpriteManager);
    SAFE_DELETE(mUIManager);
    SAFE_DELETE(mMeshManager);
}

void SceneBase::update() {
    start();
    updateScene();
    mCamera->update();
    mMeshManager->update();
    mUIManager->update();
    mSpriteManager->update();
}

void SceneBase::draw() {
    if (!mRenderer) {
        return;
    }
    mMeshManager->draw(mCamera);
    mSpriteManager->draw(mRenderer);
}

void SceneBase::nextScene(std::shared_ptr<SceneBase> next) {
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
