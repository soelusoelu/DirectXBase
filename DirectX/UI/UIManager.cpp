#include "UIManager.h"
#include "UI.h"

UIManager::UIManager() = default;
UIManager::~UIManager() = default;

void UIManager::update() {
    for (auto&& ui : mUIStack) {
        ui->update();
    }
    remove();
}

void UIManager::add(UI * add) {
    mUIStack.emplace_back(add);
}

void UIManager::clear() {
    mUIStack.clear();
}

void UIManager::remove() {
    auto itr = mUIStack.begin();
    while (itr != mUIStack.end()) {
        if ((*itr)->getState() == UIState::CLOSING) {
            itr = mUIStack.erase(itr);
        } else {
            ++itr;
        }
    }
}
