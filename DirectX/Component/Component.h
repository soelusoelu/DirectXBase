#pragma once

#include <rapidjson/document.h>
#include <memory>
#include <string>

class Actor;

class Component {
protected:
    Component(std::shared_ptr<Actor> owner, const std::string& typeName, int updateOrder = 100);
public:
    virtual ~Component();
    //getComponentはここでして
    virtual void start() {};
    virtual void update() {};
    //オーナーのTransformが更新されたら
    virtual void onUpdateWorldTransform() {};
    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj) {};
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {};

    std::shared_ptr<Actor> owner() const;
    int getUpdateOrder() const;
    const std::string& getTypeName() const;

    //指定されたプロパティでコンポーネントを生成
    template <typename T>
    static std::shared_ptr<Component> create(std::shared_ptr<Actor> actor, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>(actor);
        t->owner()->componentManager()->addComponent(t);
        t->loadProperties(inObj);
        return t;
    }

private:
    std::weak_ptr<Actor> mOwner;
    int mUpdateOrder;
    std::string mTypeName;
};

