#pragma once

#include <rapidjson/document.h>
#include <memory>

class Actor;

class Component {
protected:
    Component(std::shared_ptr<Actor> owner, int updateOrder = 100);
public:
    virtual ~Component();
    //getComponentはここでして
    virtual void start() = 0;
    virtual void update() = 0;
    //オーナーのTransformが更新されたら
    virtual void onUpdateWorldTransform() {};

    int getUpdateOrder() const;
    std::shared_ptr<Actor> owner() const;

    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj) {};
    //virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

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
};

