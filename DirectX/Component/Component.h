#pragma once

#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>
#include <utility>

class GameObject;

class Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
protected:
    using DebugInfo = std::pair<std::string, std::string>;
    using DebugInfoList = std::list<DebugInfo>;

protected:
    Component(GameObjectPtr owner, const std::string& type, int updateOrder = 100);
public:
    virtual ~Component();
    //getComponentはここでして
    virtual void start() {};
    virtual void update() {};
    //loadPropertiesの直後に呼び出される
    virtual void awake() {};
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
    virtual void drawDebugInfo(DebugInfoList* inspect) const {};

    GameObjectPtr owner() const;
    int getUpdateOrder() const;
    const std::string& getTypeName() const;

    //指定されたプロパティでコンポーネントを生成
    template <typename T>
    static void create(GameObjectPtr gameObject, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>(gameObject);
        t->owner()->componentManager()->addComponent(t);
        t->loadProperties(inObj);
        t->awake();
    }

private:
    std::weak_ptr<GameObject> mOwner;
    int mUpdateOrder;
    std::string mType;
};
