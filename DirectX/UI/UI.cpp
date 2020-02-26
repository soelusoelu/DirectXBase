#include "UI.h"
#include "UIManager.h"
#include "../Actor/Transform2D.h"
#include "../Sprite/Sprite.h"

UI::UI(const std::string& type) :
    mState(State::ACTIVE),
    mTypeName(type) {
}

UI::~UI() {
    for (auto&& sprite : mSprites) {
        sprite->destroy();
    }
}

void UI::update() {
    if (getActive()) {
        updateUI();
    }
}

void UI::close() {
    mState = State::CLOSING;
}

void UI::addSprite(SpritePtr sprite) {
    //スプライトをスプライトマネージャーに登録
    sprite->addToManager();
    mSprites.emplace_back(sprite);
}

void UI::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;

    if (value) {
        return;
    }
    //管理してる全スプライトを非アクティブに
    for (auto&& sprite : mSprites) {
        if (sprite->getActive()) {
            sprite->setActive(false);
        }
    }
}

bool UI::getActive() const {
    return mState == State::ACTIVE;
}

bool UI::isClosing() const {
    return mState == State::CLOSING;
}

const std::string& UI::getTypeName() const {
    return mTypeName;
}

void UI::setUIManager(UIManager* manager) {
    mUIManager = manager;
}

void UI::addToManager() {
    if (mUIManager) {
        mUIManager->add(shared_from_this());
    }
}

UIManager* UI::mUIManager = nullptr;
