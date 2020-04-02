#include "SceneManager.h"
#include "GamePlay.h"
#include "Title.h"
#include "../Component/Camera.h"
#include "../Component/Collider.h"
#include "../Component/ComponentManager.h"
#include "../Component/DirectionalLight.h"
#include "../DebugLayer/DebugUtility.h"
#include "../Device/DrawString.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/Game.h"

SceneManager::SceneManager(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mCurrentScene(std::make_shared<GamePlay>()),
    mCamera(nullptr),
    mGameObjectManager(new GameObjectManager()),
    mMeshManager(new MeshManager()),
    mSpriteManager(new SpriteManager()),
    mPhysics(new Physics()) {
}

SceneManager::~SceneManager() {
    SAFE_DELETE(mGameObjectManager);
    SAFE_DELETE(mMeshManager);
    SAFE_DELETE(mSpriteManager);
    SAFE_DELETE(mPhysics);
}

void SceneManager::loadProperties(const rapidjson::Value& inObj) {
}

void SceneManager::initialize() {
    GameObject::setGameObjectManager(mGameObjectManager);
    Mesh::setMeshManager(mMeshManager);
    Sprite::setSpriteManager(mSpriteManager);
    Collider::setPhysics(mPhysics);

    auto cam = GameObjectCreater::create("Camera");
    mCamera = cam->componentManager()->getComponent<Camera>();
    auto dirLight = GameObjectCreater::create("DirectionalLight");
    mDirectionalLight = dirLight->componentManager()->getComponent<DirectionalLight>();

    change();
}

void SceneManager::update() {
    //アップデートの最初で文字列削除
    mRenderer->getDrawString()->clear();
    DebugUtility::drawStringClear();

    mGameObjectManager->update();
    //現在のシーンを更新
    mCurrentScene->update();
    //総当たり判定
    mPhysics->sweepAndPrune();
    //レンダラーの更新
    mRenderer->update();
    //各マネージャークラスを更新
    mMeshManager->update();
    mSpriteManager->update();
    //デバッグ
    DebugUtility::update();

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
    mRenderer->renderFromTexture(mCamera, mDirectionalLight);
    //ポイントライトの一括描画
    mRenderer->drawPointLights();
    //透明メッシュの描画
    mMeshManager->drawTransparent(mCamera, mDirectionalLight);

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
    DebugUtility::draw(proj);
#endif // _DEBUG
}

void SceneManager::change() {
    mGameObjectManager->clear();
    mMeshManager->clear();
    mSpriteManager->clear();
    mCurrentScene->set(mRenderer, mGameObjectManager);
    mCurrentScene->start();
}
