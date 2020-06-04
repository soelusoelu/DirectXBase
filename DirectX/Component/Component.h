#pragma once

#include <rapidjson/document.h>
#include <any>
#include <list>
#include <memory>
#include <string>
#include <utility>

class GameObject;

namespace ComponentDebug {
    using DebugInfo = std::pair<std::string, std::any>;
    using DebugInfoList = std::list<DebugInfo>;
}

class Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    Component(int updateOrder = 100);
    virtual ~Component();
    //loadPropertiesの直後に呼び出される
    virtual void awake() {};
    //getComponentはここでして
    virtual void start() {};
    //毎フレーム呼ばれる
    virtual void update() {};
    //全ゲームオブジェクトの全アップデート後に呼ばれる
    virtual void lateUpdate() {};
    //終了処理
    virtual void finalize() {};
    //オーナーのTransformが更新されたら
    virtual void onUpdateWorldTransform() {};
    //アクティブ・非アクティブ時の切り替え
    virtual void onSetActive(bool value) {};
    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj) {};
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {};
    //Inspectorに表示する情報
    //first: 変数名
    //second: 値
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {};

    //コンポーネントがアタッチされているゲームオブジェクトを返す
    GameObjectPtr owner() const;
    //アップデート優先数値を返す
    int getUpdateOrder() const;
    //コンポーネントの名前を返す
    const std::string& getComponentName() const;

    //指定されたプロパティでコンポーネントを生成
    template <typename T>
    static void create(const GameObjectPtr& owner, const std::string& componentName, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>();
        t->mOwner = owner;
        t->mComponentName = componentName;
        t->owner()->componentManager()->addComponent(t);
        t->loadProperties(inObj);
        t->awake();
    }

private:
    std::weak_ptr<GameObject> mOwner;
    int mUpdateOrder;
    std::string mComponentName;
};
