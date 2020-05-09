#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_set>

class GameObject;

class GameObjectManager {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;
    using StringSet = std::unordered_set<std::string>;

public:
    GameObjectManager();
    ~GameObjectManager();
    //登録済みの全ゲームオブジェクトの更新
    void update();
    //ゲームオブジェクトの登録
    void add(const GameObjectPtr& add);
    //登録済みの全ゲームオブジェクトの削除
    void clear();
    //指定のタグを除く、登録済みの全ゲームオブジェクトの削除
    void clearExceptSpecified(const StringSet& tags);
    //ゲームオブジェクトの検索
    GameObjectPtr find(const std::string& tag) const;
    //ゲームオブジェクトのランダム検索
    GameObjectPtr randomFind(const std::string& tag) const;
    //リストの中からプレイヤーを取得
    GameObjectPtr getPlayer() const;

private:
    //ゲームオブジェクトの削除
    void remove();
    //待機中のゲームオブジェクトをメインリストに移す
    void movePendingToMain();

    //コピー禁止
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

private:
    //ゲームオブジェクトリスト
    GameObjectPtrList mGameObjects;
    GameObjectPtrList mPendingGameObjects;
    //アップデート中かのフラグ
    bool mUpdatingGameObjects;
};
