#include "UI.h"
#include "UIManager.h"
#include "../Sprite/Sprite.h"
#include <algorithm>

UI::UI() :
    mSprites(0),
    mState(UIState::ACTIVE) {
    if (mUIManager) {
        mUIManager->add(this);
    }
}

UI::~UI() {
    if (mSprites.empty()) {
        return;
    }
    for (auto&& sprite : mSprites) {
        Sprite::destroy(sprite);
    }
}

void UI::update() {
    updateUI();
}

void UI::close() {
    mState = UIState::CLOSING;
}

void UI::addSprite(Sprite* sprite) {
    mSprites.emplace_back(sprite);
}

void UI::removeSprite(Sprite* sprite) {
    auto itr = std::find(mSprites.begin(), mSprites.end(), sprite);
    if (itr != mSprites.end()) {
        mSprites.erase(itr);
    }
}

UIState UI::getState() const {
    return mState;
}

void UI::setUIManager(UIManager* manager) {
    mUIManager = manager;
}

UIManager* UI::mUIManager = nullptr;
