#pragma once

#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>
#include <utility>

class Actor;

class Component {
protected:
    using debugInfo = std::pair<std::string, std::string>;
    using debugInfoList = std::list<debugInfo>;

protected:
    Component(std::shared_ptr<Actor> owner, const std::string& typeName, int updateOrder = 100);
public:
    virtual ~Component();
    //getComponentはここでして
    virtual void start() {};
    virtual void update() {};
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
    virtual void drawDebugInfo(debugInfoList* inspect) const {};

    std::shared_ptr<Actor> owner() const;
    int getUpdateOrder() const;
    const std::string& getTypeName() const;

    //指定されたプロパティでコンポーネントを生成
    template <typename T>
    static void create(std::shared_ptr<Actor> actor, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>(actor);
        t->owner()->componentManager()->addComponent(t);
        t->loadProperties(inObj);
    }

private:
    std::weak_ptr<Actor> mOwner;
    int mUpdateOrder;
    std::string mTypeName;
};

