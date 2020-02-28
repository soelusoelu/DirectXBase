#include "UIManager.h"
#include "UI.h"
#include "../Utility/LevelLoader.h"

UIManager::UIManager() = default;
UIManager::~UIManager() {
    Singleton<LevelLoader>::instance().saveUI(mUIStack, "UIList.json");
    mUIStack.clear();
}

void UIManager::update() {
    for (auto&& ui : mUIStack) {
        ui->update();
    }
    remove();
}

void UIManager::add(UIPtr add) {
    mUIStack.emplace_back(add);
}

void UIManager::clear() {
    mUIStack.clear();
}

void UIManager::remove() {
    auto itr = mUIStack.begin();
    while (itr != mUIStack.end()) {
        if ((*itr)->isClosing()) {
            itr = mUIStack.erase(itr);
        } else {
            ++itr;
        }
    }
}
