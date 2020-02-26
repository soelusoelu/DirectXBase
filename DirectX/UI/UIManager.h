#pragma once

#include <list>
#include <memory>

class UI;

class UIManager {
    using UIPtr = std::shared_ptr<UI>;
    using UIPtrList = std::list<UIPtr>;

public:
    UIManager();
    ~UIManager();
    void update();
    void add(UIPtr add);
    void clear();

private:
    void remove();

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

private:
    UIPtrList mUIStack;
};
