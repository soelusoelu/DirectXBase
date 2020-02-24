#pragma once

#include <list>
#include <memory>

class Component;

class ComponentManager {
    using ComponentPtr = std::shared_ptr<Component>;
    using ComponentPtrList = std::list<ComponentPtr>;

public:
    ComponentManager();
    ~ComponentManager();
    //各コンポーネントのstartを一度だけ実行
    void start();
    //所有するすべてのコンポーネントを更新
    void update();
    //コンポーネントの追加
    void addComponent(ComponentPtr component);

    //所有するすべてのコンポーネントのonUpdateWorldTransformを実行
    void onUpdateWorldTransform();

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

private:
    ComponentPtrList mStartComponents;
    ComponentPtrList mComponents;
};

