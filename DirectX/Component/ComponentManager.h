#pragma once

#include <list>
#include <memory>

class Component;

class ComponentManager {
public:
    ComponentManager();
    ~ComponentManager();
    //�e�R���|�[�l���g��start����x�������s
    void start();
    //���L���邷�ׂẴR���|�[�l���g���X�V
    void update();
    //�R���|�[�l���g�̒ǉ�
    void addComponent(Component* component);

    //���L���邷�ׂẴR���|�[�l���g��onUpdateWorldTransform�����s
    void onUpdateWorldTransform();

    //�R���|�[�l���g�̎擾
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
        //�Ō�܂Ō�����Ȃ����nullptr��Ԃ�
        return comp;
    }

private:
    std::list<std::shared_ptr<Component>> mStartComponents;
    std::list<std::shared_ptr<Component>> mComponents;
};

