#include "UI.h"
#include "UIManager.h"
#include "../Actor/Transform2D.h"
#include "../Sprite/Sprite.h"

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
        sprite->destroy();
    }
}

void UI::update() {
    if (mState == UIState::ACTIVE) {
        updateUI();
    } else if (mState == UIState::NON_ACTIVE) {
        for (auto&& sprite : mSprites) {
            if (sprite->getActive()) {
                sprite->setActive(false);
            }
        }
    }
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

void UI::setActive(bool value) {
    mState = (value) ? UIState::ACTIVE : UIState::NON_ACTIVE;
}

bool UI::getActive() const {
    return mState == UIState::ACTIVE;
}

UIState UI::getState() const {
    return mState;
}

void UI::setUIManager(UIManager* manager) {
    mUIManager = manager;
}

UIManager* UI::mUIManager = nullptr;
