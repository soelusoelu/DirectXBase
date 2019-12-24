#pragma once

#include <list>
#include <memory>

class UI;

class UIManager {
public:
    UIManager();
    ~UIManager();
    void update();
    void add(UI* add);
    void clear();

private:
    void remove();

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

private:
    std::list<std::unique_ptr<UI>> mUIStack;
};
