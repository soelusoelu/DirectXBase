﻿#include "SceneManager.h"
#include "GamePlay.h"
#include "Title.h"
#include "../Camera/Camera.h"
#include "../DebugLayer/Debug.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/Game.h"
#include "../UI/UI.h"
#include "../UI/UIManager.h"

SceneManager::SceneManager(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mCurrentScene(std::make_shared<GamePlay>()),
    mCamera(std::make_shared<Camera>()),
    mMeshManager(new MeshManager()),
    mSpriteManager(new SpriteManager()),
    mUIManager(new UIManager()) {
}

SceneManager::~SceneManager() {
    SAFE_DELETE(mMeshManager);
    SAFE_DELETE(mSpriteManager);
    SAFE_DELETE(mUIManager);
}

void SceneManager::loadProperties(const rapidjson::Value& inObj) {
    mCamera->loadProperties(inObj);
}

void SceneManager::initialize() {
    Mesh::setMeshManager(mMeshManager);
    Sprite::setSpriteManager(mSpriteManager);
    UI::setUIManager(mUIManager);

    mCamera->initialize();

    change();
}

void SceneManager::update() {
    //アップデートの最初で文字列削除
    mRenderer->getDrawString()->clear();
    Debug::drawStringClear();

    //現在のシーンを更新
    mCurrentScene->update();
    //レンダラーの更新
    mRenderer->update();
    //カメラ更新
    mCamera->update();
    //各マネージャークラスを更新
    mMeshManager->update();
    mUIManager->update();
    mSpriteManager->update();
    //デバッグ
    Debug::update();

    //nullptrじゃなければシーン移行
    auto next = mCurrentScene->getNextScene();
    if (next) {
        mCurrentScene = next;
        change();
    }
}

void SceneManager::draw() const {
    //各テクスチャ上にレンダリング
    mRenderer->renderToTexture();
    //メッシュの一括描画
    mMeshManager->draw(mCamera);
    //各テクスチャを参照してレンダリング
    mRenderer->renderFromTexture(mCamera);
    //ポイントライトの一括描画
    mRenderer->drawPointLights(mCamera);

    //スプライト描画準備
    Matrix4 proj = Matrix4::identity;
    mRenderer->renderSprite(&proj);
    //スプライトの一括描画
    mSpriteManager->draw(proj);
    //テキスト一括描画
    mRenderer->getDrawString()->drawAll(proj);

#ifdef _DEBUG
    //レンダリング領域をデバッグに変更
    mRenderer->renderToDebug(&proj);
    //デバッグ表示
    Debug::draw(proj);
#endif // _DEBUG
}

void SceneManager::change() {
    mMeshManager->clear();
    mUIManager->clear();
    mSpriteManager->clear();
    mCurrentScene->set(mRenderer, mCamera);
    mCurrentScene->start();
}
