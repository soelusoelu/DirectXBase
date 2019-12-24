#include "UI.h"
#include "UIManager.h"

UI::UI() :
    //mSprites(0),
    mState(UIState::ACTIVE) {
    if (mUIManager) {
        mUIManager->add(this);
    }
}

UI::~UI() {
    //for (auto&& sprite : mSprites) {
    //    Sprite::destroy(sprite);
    //}
}

void UI::close() {
    mState = UIState::CLOSING;
}

void UI::addSprite(std::shared_ptr<Sprite> add) {
    //mSprites.emplace_back(add);
    //SpriteManager::add(add);
}

UIState UI::getState() const {
    return mState;
}

void UI::setUIManager(UIManager* manager) {
    mUIManager = manager;
}

UIManager* UI::mUIManager = nullptr;
