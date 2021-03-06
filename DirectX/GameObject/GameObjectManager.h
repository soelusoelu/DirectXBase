﻿#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class GameObject;

class GameObjectManager {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;
    using GameObjectPtrArray = std::vector<GameObjectPtr>;
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
    //tagに一致するアクティブなゲームオブジェクトの検索
    const GameObjectPtr& find(const std::string& tag) const;
    //tagに一致するアクティブな全ゲームオブジェクトの検索
    GameObjectPtrArray findGameObjects(const std::string& tag) const;

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
