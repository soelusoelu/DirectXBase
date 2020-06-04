#pragma once

#include <rapidjson/document.h>
#include <memory>
#include <string>

class ComponentManager;
class GameObjectManager;
class Renderer;
class Time;
class Transform3D;

class GameObject : public std::enable_shared_from_this<GameObject> {
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    GameObject(std::shared_ptr<Renderer> renderer);
    virtual ~GameObject();

    //更新
    void update();
    //遅延更新
    void lateUpdate();

    //削除
    void destroy();
    void destroy(float sec);

    //ロード/セーブ
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const;

    //状態操作
    virtual void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

    //タグ
    void setTag(const std::string& tag);
    const std::string& tag() const;

    //レンダラーの取得
    const std::shared_ptr<Renderer>& renderer() const;
    //トランスフォームの取得
    const std::shared_ptr<Transform3D>& transform() const;
    //コンポーネント管理者の取得
    const std::shared_ptr<ComponentManager>& componentManager() const;

    //GameObjectManagerの登録
    static void setGameObjectManager(GameObjectManager* manager);
    GameObjectManager* getGameObjectManager();

    //ゲームオブジェクトを生成
    static std::shared_ptr<GameObject> create(std::shared_ptr<Renderer> renderer);

protected:
    //ワールド行列の更新
    void computeWorldTransform();

private:
    //初期化
    void initialize();
    //DestroyTimerの更新
    void updateDestroyTimer();

protected:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<ComponentManager> mComponentManager;
    std::string mTag;

private:
    std::unique_ptr<Time> mDestroyTimer;
    State mState;

    static GameObjectManager* mGameObjectManager;
};
