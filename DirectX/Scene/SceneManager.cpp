#include "SceneManager.h"
#include "../Component/Camera.h"
#include "../Component/Collider.h"
#include "../Component/ComponentManager.h"
#include "../Component/DirectionalLight.h"
#include "../Component/MeshComponent.h"
#include "../Component/Sprite3D.h"
#include "../Component/SpriteComponent.h"
#include "../Component/Scene/Scene.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/Pause.h"
#include "../Device/DrawString.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/Game.h"

SceneManager::SceneManager(const std::shared_ptr<Renderer>& renderer) :
    mRenderer(renderer),
    mCurrentScene(nullptr),
    mCamera(nullptr),
    mGameObjectManager(new GameObjectManager()),
    mMeshManager(new MeshManager()),
    mSpriteManager(new SpriteManager()),
    mPhysics(new Physics()),
    mShouldDraw(true) {
}

SceneManager::~SceneManager() {
    SAFE_DELETE(mGameObjectManager);
    SAFE_DELETE(mMeshManager);
    SAFE_DELETE(mSpriteManager);
    SAFE_DELETE(mPhysics);

    GameObject::setGameObjectManager(nullptr);
    MeshComponent::setMeshManager(nullptr);
    Sprite::setSpriteManager(nullptr);
    SpriteComponent::setSpriteManager(nullptr);
    Sprite3D::setSpriteManager(nullptr);
    Collider::setPhysics(nullptr);
}

void SceneManager::initialize() {
    GameObject::setGameObjectManager(mGameObjectManager);
    MeshComponent::setMeshManager(mMeshManager);
    Sprite::setSpriteManager(mSpriteManager);
    SpriteComponent::setSpriteManager(mSpriteManager);
    Sprite3D::setSpriteManager(mSpriteManager);
    Collider::setPhysics(mPhysics);

    auto cam = GameObjectCreater::create("Camera");
    mCamera = cam->componentManager()->getComponent<Camera>();
    auto dirLight = GameObjectCreater::create("DirectionalLight");
    mDirectionalLight = dirLight->componentManager()->getComponent<DirectionalLight>();

    //初期シーンの設定
    createScene("Title");
}

void SceneManager::update() {
    //アップデートの最初で文字列削除
    mRenderer->getDrawString()->clear();
    DebugUtility::drawStringClear();
    mShouldDraw = true;

    //Escでゲーム終了
    if (Input::keyboard()->getKeyDown(KeyCode::Escape)) {
        Game::quit();
    }

    //ポーズ中はデバッグだけアップデートを行う
    if (DebugUtility::pause()->isPausing()) {
        DebugUtility::update();
        return;
    }

    //全ゲームオブジェクトの更新
    mGameObjectManager->update();
    //総当たり判定
    mPhysics->sweepAndPrune();
    //レンダラーの更新
    mRenderer->update();
    //各マネージャークラスを更新
    mMeshManager->update();
    mSpriteManager->update();
    //デバッグ
    DebugUtility::update();

    //シーン移行
    const auto& next = mCurrentScene->getNext();
    if (!next.empty()) {
        change(mCurrentScene->getObjectToNext());
        createScene(next);
        mShouldDraw = false;
    }
}

void SceneManager::draw() const {
    if (!mShouldDraw) {
        return;
    }
    //各テクスチャ上にレンダリング
    mRenderer->renderToTexture();
    //メッシュの一括描画
    mMeshManager->draw(*mCamera);
    //各テクスチャを参照してレンダリング
    mRenderer->renderFromTexture(*mCamera, *mDirectionalLight);
    //ポイントライトの一括描画
    mRenderer->drawPointLights();
    //透明メッシュの描画
    mMeshManager->drawTransparent(*mCamera);

    //スプライト描画準備
    mRenderer->renderSprite();
    //3Dスプライト
    mRenderer->renderSprite3D();
    mSpriteManager->draw3Ds(mCamera->getViewProjection());
    //2Dスプライト
    auto proj = Matrix4::identity;
    mRenderer->renderSprite2D(&proj);
    mSpriteManager->drawComponents(proj);
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

void SceneManager::change(const StringSet& tags) {
    mGameObjectManager->clearExceptSpecified(tags);
    mMeshManager->clear();
    mSpriteManager->clear();
}

void SceneManager::createScene(const std::string& name) {
    auto scene = GameObjectCreater::create(name);
    mCurrentScene = scene->componentManager()->getComponent<Scene>();
}
