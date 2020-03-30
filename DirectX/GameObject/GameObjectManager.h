﻿#pragma once

#include <list>
#include <memory>
#include <string>

class GameObject;

class GameObjectManager {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    GameObjectManager();
    ~GameObjectManager();
    //登録済みの全ゲームオブジェクトの更新
    void update();
    //ゲームオブジェクトの登録
    void add(GameObjectPtr add);
    //登録済みの全ゲームオブジェクトの削除
    void clear();
    GameObjectPtr find(const std::string& tag) const;
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