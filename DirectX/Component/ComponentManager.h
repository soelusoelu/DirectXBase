#pragma once

#include <list>
#include <memory>
#include <string>

class Actor;
class Component;

class ComponentManager {
    using ComponentPtr = std::shared_ptr<Component>;
    using ComponentPtrList = std::list<ComponentPtr>;

public:
    ComponentManager(std::shared_ptr<Actor> owner);
    ~ComponentManager();
    //各コンポーネントのstartを一度だけ実行
    void start();
    //所有するすべてのコンポーネントを更新
    void update();
    //コンポーネントの追加
    void addComponent(ComponentPtr component);

    template<typename T>
    void addComponent() {
        mStartComponents.emplace_back(std::make_shared<T>(mOwner.lock()));
    }

    //所有するすべてのコンポーネントのonUpdateWorldTransformを実行
    void onUpdateWorldTransform();
    //所有するすべてのコンポーネントのonSetActiveを実行
    void onSetActive(bool value);

    //全コンポーネントの取得
    ComponentPtrList getAllComponents() const;
    //コンポーネントの取得
    template<typename T>
    std::shared_ptr<T> getComponent() const {
        std::shared_ptr<T> comp = nullptr;
        for (const auto& c : mStartComponents) {
            comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                return comp;
            }
        }
        for (const auto& c : mComponents) {
            comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                break;
            }
        }
        //最後まで見つからなければnullptrを返す
        return comp;
    }

    //コンポーネントを文字列から取得
    std::shared_ptr<Component> getComponent(const std::string& type) const;

private:
    std::weak_ptr<Actor> mOwner;
    ComponentPtrList mStartComponents;
    ComponentPtrList mComponents;
};

